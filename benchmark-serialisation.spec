###########################################################################
# Copyright 2015 - 2020 IoT.bzh
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
###########################################################################
Name:    benchmark-serialization
#Hexsha: 44b26fba2a2e7d2dcbb2ff9361b903d22303ef54
Version: 1.0.0
Release: 0%{?dist}
License: APL2.0
Summary: benchmark serialization
URL:     http://git.ovh.iot/redpesk-labs/benchmark-serialization.git
Source0: %{name}-%{version}.tar.gz

BuildRequires: cmake
BuildRequires: gcc gcc-c++
BuildRequires: pkgconfig(json-c)
BuildRequires: pkgconfig(libfastjson)
BuildRequires: pkgconfig(libcbor)
BuildRequires: pkgconfig(libprotobuf-c)

%description
A benchmark fo differents techno/library for serialize/parse data

%prep
%autosetup -p 1

%build
mkdir %{_target_platform}
cmake -B %{_target_platform}
make -C %{_target_platform}


%install
%make_install -C %{_builddirpkg}

%check

%clean

%files
/usr/bin/benchmark_cbor
/usr/bin/benchmark_fastjson
/usr/bin/benchmark_jsonc
/usr/bin/benchmark_jsoncpp
/usr/bin/benchmark_jsonstring
/usr/bin/benchmark_jsmn
/usr/bin/benchmark_protobuf
/usr/bin/benchmark_xdr

%changelog

* Wed Jun 24 2020 IoT.bzh(iotpkg) <redpesk.list@iot.bzh> 1.0.0
- Upgrade version from source commit sha: 44b26fba2a2e7d2dcbb2ff9361b903d22303ef54
- Commit message:
- 	casting time change


* Wed Jun 24 2020 IoT.bzh(iotpkg) <redpesk.list@iot.bzh> 1.0.0
- Upgrade version from source commit sha: 6c6a90b1cdb2d81a394ae7632cbaad91eaf5820d
- Commit message:
- 	add screen profiling


* Wed Jun 24 2020 IoT.bzh(iotpkg) <redpesk.list@iot.bzh> 1.0.0
- Upgrade version from source commit sha: 6c6a90b1cdb2d81a394ae7632cbaad91eaf5820d
- Commit message:
- 	add screen profiling


* Fri Jun 19 2020 IoT.bzh(iotpkg) <redpesk.list@iot.bzh> 1.0.0
- Upgrade version from source commit sha: 0f3f2de715a06f7454584a2cee776259b2a96268
- Commit message:
- 	add cmake installation


* Fri Jun 19 2020 IoT.bzh(iotpkg) <redpesk.list@iot.bzh> 1.0.0
- Upgrade version from source commit sha: 52dcb4e4cb293d6c2f134d92eb2163b43d9695cf
- Commit message:
- 	change include for rpc and modification CMAKE


* Fri Jun 19 2020 IoT.bzh(iotpkg) <redpesk.list@iot.bzh> 1.0.0
- Upgrade version from source commit sha: 52dcb4e4cb293d6c2f134d92eb2163b43d9695cf
- Commit message:
- 	change include for rpc and modification CMAKE


* Fri Jun 19 2020 IoT.bzh(iotpkg) <redpesk.list@iot.bzh> 1.0.0
- Upgrade version from source commit sha: e9ed2b0c3b868ee542eb05999c04c4bd22897c21
- Commit message:
- 	update cmakelists with pkg_check_module 2

* Mon May 18 2020 Valentin Lefebvre <valentin.lefebvre@iot.bzh> 1.0.0
- Creation of the specfile
