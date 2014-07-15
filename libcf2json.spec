Name:           libcf2json
Version:        1.0.0
Release:        2
License:        unknown
Group:          unknown
Vendor:         RuToll.RU
Packager:       Sergey Khvatov<s.xbatob@gmail.com>
BuildRoot:      %{_tmppath}/%{name}-root
Source:         http://sagittarius.rutoll.ru/%{name}-%{version}.tar.gz
Summary:        convert configuration from format libconfig++ to json

%description
DESCRIPTION
===========
convert configuration from format libconfig++ to json one

%prep
%setup

%build
%cmake
make

%install
rm -rf %{buildroot}
%make_install

%files
%defattr(-,root,root,-)
%{_bindir}/*

%changelog
