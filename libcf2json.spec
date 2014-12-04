Name:           libcf2json
Version:        1.1.0
Release:        1
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
* Thu Dec 4 2014 - Serguei Khvatov <xbatob@rutoll.ru>

- поменял версию
- изменил поведение для возможности слияния нескольких конфигураций

* Tue Jul 15 2014 - Serguei Khvatov <xbatob@rutoll.ru>

- добавил полезные цели в cmakelists

* Wed Nov 13 2013 - Serguei Khvatov <xbatob@rutoll.ru>

- прописал spec-файл вручную

* Tue Nov 12 2013 - Serguei Khvatov <xbatob@rutoll.ru>

- Добавил поддержку CPack (правда, от него толку немного: он не умеет собирать src.rpm)

* Tue Oct 8 2013 - Serguei Khvatov <xbatob@rutoll.ru>

- улучшена диагностика

* Fri Jul 26 2013 - Serguei Khvatov <S.XBATOB@gmail.com>

- добаыил возможность коныертировать поддерево

* Tue May 28 2013 - Serguei Khvatov <S.XBATOB@gmail.com>

- пустые группы и массивы превращались в null
- переделал в конфертор в JSON

* Mon May 6 2013 - Serguei Khvatov <S.XBATOB@gmail.com>

- выходной файл не очищался Зато сейчас он очищается без предупреждения!
- BUG: пропущен break в разборе параметров
- улучшение диагностики
- добавил вывод результата в файл
- initial revision

