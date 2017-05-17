Name:		not24get
Version:	0.2
Release:	1%{?dist}
Summary:	not24get is a module for password quality checking in OpenLDAP to be used together with ppolicy.

License:	GPL
URL:		https://github.com/tomato42/not24get
Source0:	not24get-0.2.tar.gz
BuildRoot:	%(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)

BuildRequires:	passwdqc-devel openldap-devel
Requires:	passwdqc-lib

%description
not24get is a module for password quality checking in OpenLDAP to be used
together with ppolicy.

It implements the API needed for use as a pwdCheckModule object in ppolicy.
It also provides an executable that can be used as "check password script" in
Samba suite (not24get_check).

This module will check quality of password based on similar rationale as
was shown in http://xkcd.com/936/, it's a wrapper around libpasswdqc, the
library that powers the pam_passwdqc module.

As password composed of few random common words is easy to remember and safer
than most passwords that get through the regular 3 out of 4 types of character
sets rule, the default configuration requires fairly long passwords.


%prep
%setup -q


%build
make %{?_smp_mflags}


%install
rm -rf %{buildroot}
mkdir -p %{buildroot}/usr/lib64/openldap/
mkdir -p %{buildroot}/etc/
install -p -m 755 %{_builddir}/%{name}-%{version}/libnot24get.so %{buildroot}/usr/lib64/openldap/libnot24get.so
install -p -m 755 %{_builddir}/%{name}-%{version}/not24get_check %{buildroot}/usr/bin/not24get_check
install -p -m 755 %{_builddir}/%{name}-%{version}/not24get.so %{buildroot}/usr/lib64/openldap/not24get.so
install -p -m 644 %{_builddir}/%{name}-%{version}/not24get.conf %{buildroot}/etc/not24get.conf


%clean
rm -rf %{buildroot}


%files
%defattr(-,root,root,-)
%doc
/usr/lib64/openldap/libnot24get.so
/usr/lib64/openldap/not24get_check
/usr/lib64/openldap/not24get.so
/etc/not24get.conf


%changelog
* Tue May 16 2017 Mozilla <kang@mozilla.com> - 0.2-1
- Use a Mozilla packaged release of the master branch (called 0.2)

* Tue May 16 2017 Mozilla <kang@mozilla.com> - 0.1-2
- Ported from internal Mozilla spec file
