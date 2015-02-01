#git:framework/web/wrt-plugins-common
Name:       wrt-plugins-common
Summary:    wrt-plugins common library
Version:    0.3.132_w3
Release:    1
Group:      Development/Libraries
License:    Apache License, Version 2.0
Source0:    %{name}-%{version}.tar.gz

BuildRequires: cmake
BuildRequires: pkgconfig(dpl-efl)
BuildRequires: pkgconfig(wrt-plugins-types)
BuildRequires: pkgconfig(security-client)
BuildRequires: pkgconfig(dpl-event-efl)
BuildRequires: pkgconfig(ewebkit2)
BuildRequires: pkgconfig(dpl-wrt-dao-ro)
BuildRequires: pkgconfig(dpl-wrt-dao-rw)
BuildRequires: pkgconfig(libpcrecpp)
BuildRequires: pkgconfig(icu-i18n)
BuildRequires: pkgconfig(libxml-2.0)
BuildRequires: pkgconfig(cert-svc-vcore)
BuildRequires: pkgconfig(wrt-commons-widget-interface-dao)
Requires:  ldconfig

%description
JavaScript plugins for WebRuntime

%package devel
Summary:    wrt-plugins-common library development headers
Group:      Development/Libraries
Requires:   %{name} = %{version}

%description devel
wrt-plugins common library development headers

%prep
%setup -q

%define with_tests 0
%if "%{WITH_TESTS}" == "ON" || "%{WITH_TESTS}" == "Y" || "%{WITH_TESTS}" == "YES" || "%{WITH_TESTS}" == "TRUE" || "%{WITH_TESTS}" == "1"
    %define with_tests 1
%endif

%build
export CFLAGS="$CFLAGS -DTIZEN_DEBUG_ENABLE"
export CXXFLAGS="$CXXFLAGS -DTIZEN_DEBUG_ENABLE"
export FFLAGS="$FFLAGS -DTIZEN_DEBUG_ENABLE"

export LDFLAGS+="-Wl,--rpath=/usr/lib -Wl,--as-needed -Wl,--hash-style=both"

cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix}  \
        -DDPL_LOG="ON"                     \
        -DCMAKE_PROJECT_VERSION=%{version} \
        -DCMAKE_BUILD_TYPE=%{?build_type:%build_type} \
        %{?WITH_TESTS:-DWITH_TESTS=%WITH_TESTS}
make %{?jobs:-j%jobs}

%install
mkdir -p %{buildroot}/usr/share/license
cp LICENSE %{buildroot}/usr/share/license/%{name}
%make_install

%clean
rm -rf %{buildroot}

%post
mkdir -p %{_libdir}/wrt-plugins
mkdir -p /usr/etc/wrt/injected-javascript
touch /opt/share/widget/plugin-installation-required

%files
%manifest wrt-plugins-common.manifest
/usr/etc/wrt-plugins/config.dtd
%{_libdir}/*.so
%{_libdir}/*.so.*
%{_libdir}/wrt-plugins/w3c-widget-interface/libwrt-plugins-w3c-widget-interface.so
%attr(755,root,root) %{_bindir}/wrt-popup-ace-runtime
%attr(755,root,root) %{_bindir}/wrt-popup-wrt-runtime
%exclude %attr(755,root,root) %{_bindir}/wrt-plugins-installer
%{_datadir}/license/%{name}
/etc/smack/accesses.d/wrt-popup-ace-runtime.efl
/etc/smack/accesses.d/wrt-popup-wrt-runtime.efl
%if %{with_tests}
    %attr(755,root,root) %{_bindir}/widgetdb_tests_prepare_db.sh
    %attr(755,root,root) %{_bindir}/wrt-plugins-common-tests-widgetdb
%endif

%files devel
%{_includedir}/*
%{_libdir}/pkgconfig/*
