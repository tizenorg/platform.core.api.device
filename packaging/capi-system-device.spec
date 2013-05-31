Name:       capi-system-device
Summary:    A Device library in TIZEN C API
Version:    0.1.0
Release:    19
Group:      System/API
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
Source1:    capi-system-device.manifest
BuildRequires:  cmake
BuildRequires:  pkgconfig(devman)
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(vconf)

%description
A Device library in TIZEN C API.


%package devel
Summary:  A Device library in TIZEN C API (Development)
Group:    System/Development
Requires: %{name} = %{version}-%{release}

%description devel
%devel_desc



%prep
%setup -q


%build
cp %{SOURCE1} .
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
%cmake . -DFULLVER=%{version} -DMAJORVER=${MAJORVER}

make %{?jobs:-j%jobs}

%install
%make_install

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%license LICENSE
%manifest capi-system-device.manifest
%{_libdir}/libcapi-system-device.so.*

%files devel
%{_includedir}/system/device.h
%{_libdir}/pkgconfig/*.pc
%{_libdir}/libcapi-system-device.so
