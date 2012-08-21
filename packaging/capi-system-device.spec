Name:       capi-system-device
Summary:    A Device library in TIZEN C API
Version: 0.1.0
Release:    8
Group:      TO_BE/FILLED_IN
License:    TO BE FILLED IN
Source0:    %{name}-%{version}.tar.gz
BuildRequires:  cmake
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(devman)
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(vconf)

Requires(post): /sbin/ldconfig  
Requires(postun): /sbin/ldconfig

%description


%package devel
Summary:  A Device library in TIZEN C API (Development)
Group:    TO_BE/FILLED_IN
Requires: %{name} = %{version}-%{release}

%description devel



%prep
%setup -q


%build
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
cmake . -DCMAKE_INSTALL_PREFIX=/usr -DFULLVER=%{version} -DMAJORVER=${MAJORVER}

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%{_libdir}/libcapi-system-device.so.*

%files devel
%{_includedir}/system/device.h
%{_libdir}/pkgconfig/*.pc
%{_libdir}/libcapi-system-device.so


