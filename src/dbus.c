/*
 * system-dbus
 *
 * Copyright (c) 2012 - 2013 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	 http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

#include "common.h"
#include "dbus.h"

#define DBUS_REPLY_TIMEOUT	(-1)

struct pending_call_data {
	dbus_pending_cb func;
	void *data;
};

static int g_dbus_error_to_errno(int code)
{
	/**
	 * if device is not supported,
	 * deviced does not register the method call of the device.
	 * in this case, dbus will return UNKNOWN_METHOD error.
	 */
	/* refer to gio/gioenums.h */
	if (code == G_DBUS_ERROR_ACCESS_DENIED)
		return -EACCES;
	else if (code == G_DBUS_ERROR_UNKNOWN_METHOD)
		return -ENOTSUP;
	return -ECOMM;
}

static GVariant *append_g_variant(const char *sig, char *param[])
{
	GVariantBuilder builder;
	GVariantBuilder *sub_builder;
	GVariant *var;
	struct dbus_int *array_int;
	char *ch;
	int i, j;

	if (!sig || !param)
		return NULL;

	if (!g_variant_type_string_is_valid(sig)) {
		_D("type string is invalid");
		return NULL;
	}

	g_variant_builder_init(&builder, G_VARIANT_TYPE_TUPLE);

	for (ch = (char*)sig, i = 0; *ch != '\0'; ++i, ++ch) {
		switch (*ch) {
		case 'i':
			g_variant_builder_add(&builder, "i", atoi(param[i]));
			break;
		case 'u':
			g_variant_builder_add(&builder, "u", strtoul(param[i], NULL, 10));
			break;
		case 't':
			g_variant_builder_add(&builder, "t", atoll(param[i]));
			break;
		case 's':
			g_variant_builder_add(&builder, "s", param[i]);
			break;
		case 'a':
			++ch;
			switch (*ch) {
			case 'i':
				sub_builder = g_variant_builder_new(G_VARIANT_TYPE("ai"));
				array_int = (struct dbus_int *)param[i];
				for (j = 0; j < array_int->size; j++)
					g_variant_builder_add(sub_builder, "i", array_int->list[j]);
				var = g_variant_new("ai", sub_builder);
				g_variant_builder_unref(sub_builder);
				g_variant_builder_add_value(&builder, var);
				break;
			default:
				break;
			}
			break;
		default:
			return NULL;
		}
	}

	return g_variant_builder_end(&builder);
}

int dbus_method_sync(const char *dest, const char *path,
		const char *interface, const char *method,
		const char *sig, char *param[])
{
	GDBusConnection *conn;
	GDBusProxy *proxy;
	GError *err = NULL;
	GVariant *output;
	int result;

#if !GLIB_CHECK_VERSION(2, 35, 0)
	g_type_init();
#endif

	conn = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &err);
	if (!conn) {
		_E("g_bus_get_sync error : %s-%s (%d-%s)",
				interface, method, err->code, err->message);
		result = g_dbus_error_to_errno(err->code);
		g_clear_error(&err);
		return result;
	}

	proxy = g_dbus_proxy_new_sync(conn,
			G_DBUS_PROXY_FLAGS_NONE,
			NULL,      /* GDBusinterfaceinfo */
			dest,      /* bus name */
			path,      /* object path */
			interface, /* interface name */
			NULL,      /* GCancellable */
			&err);
	if (!proxy) {
		_E("g_dbus_proxy_new_sync error : %s-%s (%d-%s)",
				interface, method, err->code, err->message);
		result = g_dbus_error_to_errno(err->code);
		g_clear_error(&err);
		return result;
	}

	output = g_dbus_proxy_call_sync(proxy,
			method,                       /* method name */
			append_g_variant(sig, param), /* parameters */
			G_DBUS_CALL_FLAGS_NONE,
			DBUS_REPLY_TIMEOUT,           /* timeout */
			NULL,                         /* GCancellable */
			&err);
	if (!output) {
		if (!err) {
			_E("g_dbus_proxy_call_sync error : %s-%s",
					interface, method);
			g_object_unref(proxy);
			return result;
		}
		_E("g_dbus_proxy_call_sync error : %s-%s (%d-%s)",
				interface, method, err->code, err->message);
		result = g_dbus_error_to_errno(err->code);
		g_clear_error(&err);
		g_object_unref(proxy);
		return result;
	}

	/* get output value */
	g_variant_get(output, "(i)", &result);

	g_variant_unref(output);
	g_object_unref(proxy);

	return result;
}

static void cb_pending(GDBusProxy *proxy,
		GAsyncResult *res,
		gpointer user_data)
{
	struct pending_call_data *data = user_data;
	GError *err = NULL;
	GVariant *output;

	output = g_dbus_proxy_call_finish(proxy,
			res, /* GAsyncResult */
			&err);
	if (!output)
		_E("g_dbus_proxy_call_finish error : %d-%s",
				err->code, err->message);

	if (data && data->func)
		data->func(data->data, output, err);

	if (err)
		g_clear_error(&err);
	if (output)
		g_variant_unref(output);
	g_object_unref(proxy);
}

int dbus_method_async_with_reply(const char *dest, const char *path,
		const char *interface, const char *method,
		const char *sig, char *param[],
		dbus_pending_cb cb, int timeout, void *data)
{
	GDBusConnection *conn;
	GDBusProxy *proxy;
	GError *err = NULL;
	struct pending_call_data *pdata;
	int result;

#if !GLIB_CHECK_VERSION(2, 35, 0)
	g_type_init();
#endif

	conn = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &err);
	if (!conn) {
		_E("g_bus_get_sync error : %s-%s (%d-%s)",
				interface, method, err->code, err->message);
		result = g_dbus_error_to_errno(err->code);
		g_clear_error(&err);
		return result;
	}

	proxy = g_dbus_proxy_new_sync(conn,
			G_DBUS_PROXY_FLAGS_NONE,
			NULL,      /* GDBusinterfaceinfo */
			dest,      /* bus name */
			path,      /* object path */
			interface, /* interface name */
			NULL,      /* GCancellable */
			&err);
	if (!proxy) {
		_E("g_dbus_proxy_new_sync error : %s-%s (%d-%s)",
				interface, method, err->code, err->message);
		result = g_dbus_error_to_errno(err->code);
		g_clear_error(&err);
		return result;
	}

	pdata = malloc(sizeof(struct pending_call_data));
	if (!pdata) {
		_E("malloc error : %s-%s",
				interface, method);
		return -ENOMEM;
	}

	pdata->func = cb;
	pdata->data = data;

	g_dbus_proxy_call(proxy,
			method,                          /* method name */
			append_g_variant(sig, param),    /* parameters */
			G_DBUS_CALL_FLAGS_NONE,
			DBUS_REPLY_TIMEOUT,              /* timeout */
			NULL,                            /* GCancellable */
			(GAsyncReadyCallback)cb_pending, /* GAsyncReadyCallback */
			pdata);                          /* user data */

	return 0;
}
