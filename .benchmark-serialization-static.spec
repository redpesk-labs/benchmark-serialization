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
Name:    benchmark-serialization-static
#Hexsha: 7077ab884fe44410ad9c629a46d72a831c40d009
Version: 1.0.0
Release: 0%{?dist}
License: APL2.0
Summary: benchmark serialization static mode
URL:     http://git.ovh.iot/redpesk-labs/benchmark-serialization.git
Source0: %{name}-%{version}.tar.gz

BuildRequires: cmake
BuildRequires: gcc gcc-c++
BuildRequires: pkgconfig(json-c)
BuildRequires: pkgconfig(libfastjson)
BuildRequires: pkgconfig(libcbor)
BuildRequires: pkgconfig(libprotobuf-c)

%description
A benchmark fo differents techno/library for serialize/parse data in static mode 

%prep
%autosetup -p 1

%build
mkdir %{_target_platform}
cmake -DBENCH_JSON_ARRAY=ON -DBENCH_FASTJSON_ARRAY=ON -DBENCH_JSONSTRING_ARRAY=ON -DBENCH_CBOR_ARRAY=ON -B %{_target_platform} 
make -C %{_target_platform}

%install
%make_install -C %{_builddirpkg}

%check

%clean

%files
/usr/bin/benchmark_cbor_array
/usr/bin/benchmark_fastjson_array
/usr/bin/benchmark_jsonc_array
/usr/bin/benchmark_jsonstring_array
/usr/bin/benchmark_protobuf
/usr/bin/benchmark_xdr

%changelog

* Wed Jun 24 2020 IoT.bzh(iotpkg) <redpesk.list@iot.bzh> 1.0.0
- Upgrade version from source commit sha: 7077ab884fe44410ad9c629a46d72a831c40d009
- Commit message:
- 	gitignore .tar


* Wed Jun 24 2020 IoT.bzh(iotpkg) <redpesk.list@iot.bzh> 1.0.0
- Upgrade version from source commit sha: df30111ddb207e6a78aa6a8319b5b7715ed01f15
- Commit message:
- 	fix side effect with timer ref C


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
