# Graph Report - BrainGenix-API  (2026-05-24)

## Corpus Check
- 193 files · ~117,335 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 1211 nodes · 1296 edges · 211 communities (181 shown, 30 thin omitted)
- Extraction: 100% EXTRACTED · 0% INFERRED · 0% AMBIGUOUS · INFERRED: 5 edges (avg confidence: 0.8)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `7692400b`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- [[_COMMUNITY_Community 1|Community 1]]
- [[_COMMUNITY_Community 2|Community 2]]
- [[_COMMUNITY_Community 3|Community 3]]
- [[_COMMUNITY_Community 4|Community 4]]
- [[_COMMUNITY_Community 5|Community 5]]
- [[_COMMUNITY_Community 6|Community 6]]
- [[_COMMUNITY_Community 7|Community 7]]
- [[_COMMUNITY_Community 8|Community 8]]
- [[_COMMUNITY_Community 9|Community 9]]
- [[_COMMUNITY_Community 10|Community 10]]
- [[_COMMUNITY_Community 11|Community 11]]
- [[_COMMUNITY_Community 12|Community 12]]
- [[_COMMUNITY_Community 13|Community 13]]
- [[_COMMUNITY_Community 14|Community 14]]
- [[_COMMUNITY_Community 15|Community 15]]
- [[_COMMUNITY_Community 16|Community 16]]
- [[_COMMUNITY_Community 17|Community 17]]
- [[_COMMUNITY_Community 18|Community 18]]
- [[_COMMUNITY_Community 19|Community 19]]
- [[_COMMUNITY_Community 20|Community 20]]
- [[_COMMUNITY_Community 21|Community 21]]
- [[_COMMUNITY_Community 22|Community 22]]
- [[_COMMUNITY_Community 23|Community 23]]
- [[_COMMUNITY_Community 24|Community 24]]
- [[_COMMUNITY_Community 25|Community 25]]
- [[_COMMUNITY_Community 26|Community 26]]
- [[_COMMUNITY_Community 27|Community 27]]
- [[_COMMUNITY_Community 28|Community 28]]
- [[_COMMUNITY_Community 29|Community 29]]
- [[_COMMUNITY_Community 30|Community 30]]
- [[_COMMUNITY_Community 31|Community 31]]
- [[_COMMUNITY_Community 32|Community 32]]
- [[_COMMUNITY_Community 33|Community 33]]
- [[_COMMUNITY_Community 34|Community 34]]
- [[_COMMUNITY_Community 35|Community 35]]
- [[_COMMUNITY_Community 36|Community 36]]
- [[_COMMUNITY_Community 37|Community 37]]
- [[_COMMUNITY_Community 38|Community 38]]
- [[_COMMUNITY_Community 39|Community 39]]
- [[_COMMUNITY_Community 40|Community 40]]
- [[_COMMUNITY_Community 41|Community 41]]
- [[_COMMUNITY_Community 42|Community 42]]
- [[_COMMUNITY_Community 43|Community 43]]
- [[_COMMUNITY_Community 44|Community 44]]
- [[_COMMUNITY_Community 45|Community 45]]
- [[_COMMUNITY_Community 46|Community 46]]
- [[_COMMUNITY_Community 47|Community 47]]
- [[_COMMUNITY_Community 48|Community 48]]
- [[_COMMUNITY_Community 49|Community 49]]
- [[_COMMUNITY_Community 50|Community 50]]
- [[_COMMUNITY_Community 51|Community 51]]
- [[_COMMUNITY_Community 52|Community 52]]
- [[_COMMUNITY_Community 53|Community 53]]
- [[_COMMUNITY_Community 54|Community 54]]
- [[_COMMUNITY_Community 55|Community 55]]
- [[_COMMUNITY_Community 56|Community 56]]
- [[_COMMUNITY_Community 57|Community 57]]
- [[_COMMUNITY_Community 58|Community 58]]
- [[_COMMUNITY_Community 59|Community 59]]
- [[_COMMUNITY_Community 60|Community 60]]
- [[_COMMUNITY_Community 61|Community 61]]
- [[_COMMUNITY_Community 62|Community 62]]
- [[_COMMUNITY_Community 63|Community 63]]
- [[_COMMUNITY_Community 64|Community 64]]
- [[_COMMUNITY_Community 65|Community 65]]
- [[_COMMUNITY_Community 66|Community 66]]
- [[_COMMUNITY_Community 68|Community 68]]
- [[_COMMUNITY_Community 72|Community 72]]
- [[_COMMUNITY_Community 73|Community 73]]
- [[_COMMUNITY_Community 75|Community 75]]
- [[_COMMUNITY_Community 76|Community 76]]
- [[_COMMUNITY_Community 77|Community 77]]
- [[_COMMUNITY_Community 78|Community 78]]
- [[_COMMUNITY_Community 79|Community 79]]
- [[_COMMUNITY_Community 80|Community 80]]
- [[_COMMUNITY_Community 81|Community 81]]
- [[_COMMUNITY_Community 82|Community 82]]
- [[_COMMUNITY_Community 83|Community 83]]
- [[_COMMUNITY_Community 84|Community 84]]
- [[_COMMUNITY_Community 85|Community 85]]
- [[_COMMUNITY_Community 86|Community 86]]
- [[_COMMUNITY_Community 89|Community 89]]
- [[_COMMUNITY_Community 90|Community 90]]
- [[_COMMUNITY_Community 92|Community 92]]
- [[_COMMUNITY_Community 93|Community 93]]
- [[_COMMUNITY_Community 102|Community 102]]
- [[_COMMUNITY_Community 103|Community 103]]
- [[_COMMUNITY_Community 105|Community 105]]
- [[_COMMUNITY_Community 107|Community 107]]
- [[_COMMUNITY_Community 108|Community 108]]
- [[_COMMUNITY_Community 114|Community 114]]
- [[_COMMUNITY_Community 139|Community 139]]
- [[_COMMUNITY_Community 148|Community 148]]
- [[_COMMUNITY_Community 157|Community 157]]
- [[_COMMUNITY_Community 163|Community 163]]
- [[_COMMUNITY_Community 164|Community 164]]
- [[_COMMUNITY_Community 165|Community 165]]
- [[_COMMUNITY_Community 166|Community 166]]
- [[_COMMUNITY_Community 171|Community 171]]
- [[_COMMUNITY_Community 172|Community 172]]
- [[_COMMUNITY_Community 173|Community 173]]
- [[_COMMUNITY_Community 174|Community 174]]
- [[_COMMUNITY_Community 175|Community 175]]
- [[_COMMUNITY_Community 176|Community 176]]
- [[_COMMUNITY_Community 177|Community 177]]
- [[_COMMUNITY_Community 180|Community 180]]
- [[_COMMUNITY_Community 181|Community 181]]

## God Nodes (most connected - your core abstractions)
1. `SignalHandling` - 17 edges
2. `VSDA` - 15 edges
3. `Printer` - 14 edges
4. `Routes` - 14 edges
5. `Routes` - 14 edges
6. `VSDA` - 13 edges
7. `TraceResolverLinuxBase` - 12 edges
8. `TraceResolverLinuxImpl<trace_resolver_tag::libbfd>` - 12 edges
9. `handle` - 11 edges
10. `StackTraceImplBase` - 11 edges

## Surprising Connections (you probably didn't know these)
- `picojson()` --calls--> `is()`  [INFERRED]
  ThirdParty/jwt-cpp/include/picojson/picojson.h → ThirdParty/jwt-cpp/include/jwt-cpp/traits/danielaparker-jsoncons/traits.h
- `as_object()` --calls--> `object_type()`  [INFERRED]
  ThirdParty/jwt-cpp/include/jwt-cpp/traits/danielaparker-jsoncons/traits.h → ThirdParty/jwt-cpp/include/jwt-cpp/traits/open-source-parsers-jsoncpp/traits.h
- `SubclassExample` --inherits--> `Example`  [EXTRACTED]
  Docs/Doxygen/Third-Party/doxygen-awesome-css/include/MyLibrary/subclass-example.hpp → Docs/Doxygen/Third-Party/doxygen-awesome-css/include/MyLibrary/example.hpp

## Communities (211 total, 30 thin omitted)

### Community 1 - "Community 1"
Cohesion: 0.05
Nodes (41): API, As a subdirectory:, code:block1 (add_subdirectory(/path/to/backward-cpp)), code:c++ (class Printer { public:), code:c++ (backward::SignalHandling sh;), code:c++ (bool loaded() const // true if loaded with success), code:c++ (struct Trace {), code:c++ (struct ResolvedTrace: public Trace {) (+33 more)

### Community 2 - "Community 2"
Cohesion: 0.06
Nodes (6): as_integer(), as_number(), as_object(), get_type(), as_object(), object_type()

### Community 3 - "Community 3"
Cohesion: 0.07
Nodes (13): as_array(), as_string(), get_audience(), get_header_claim(), get_payload_claim(), has_header_claim(), has_payload_claim(), verify() (+5 more)

### Community 4 - "Community 4"
Cohesion: 0.08
Nodes (25): backtrace_symbol, darwin_tag, default_delete, demangler, demangler_impl, die_call_file(), get_type(), get_type_by_signature() (+17 more)

### Community 5 - "Community 5"
Cohesion: 0.07
Nodes (28): Browser support, Class Diagrams with Graphviz, code:bash (git submodule add https://github.com/jothepro/doxygen-awesom), code:block10 (HTML_EXTRA_STYLESHEET  = doxygen-awesome-theme/doxygen-aweso), code:css (/* custom.css */), code:block12 (# Doxyfile), code:block13 (# Doxyfile), code:block2 (# Doxyfile) (+20 more)

### Community 6 - "Community 6"
Cohesion: 0.11
Nodes (19): is(), float, deny_parse_context(), parse_array_start(), parse_array_stop(), parse_object_start(), parse_object_stop(), picojson() (+11 more)

### Community 7 - "Community 7"
Cohesion: 0.10
Nodes (14): Colorize, _enabled, _reset, Printer, address, color_mode, inliner_context_size, object (+6 more)

### Community 8 - "Community 8"
Cohesion: 0.14
Nodes (13): exception, AssertFailedError, basename, errmsg, line, error(), getprogname(), main() (+5 more)

### Community 9 - "Community 9"
Cohesion: 0.11
Nodes (10): ResolvedTrace, inliners, object_filename, object_function, source, StackTrace, StackTraceImpl, Trace (+2 more)

### Community 10 - "Community 10"
Cohesion: 0.11
Nodes (17): Base64 Options, code:cpp (#include <jwt-cpp/jwt.h>), code:sh (cmake .), code:cpp (auto verifier = jwt::verify()), code:cpp (auto token = jwt::create()), code:cpp (jwt::basic_claim<my_favorite_json_library_traits> claim(json), Conference Coverage, Configuration Options (+9 more)

### Community 11 - "Community 11"
Cohesion: 0.25
Nodes (11): deep_first_search_by_pc(), die_has_pc(), find_die(), find_fundie_by_pc(), get_referenced_die(), get_referenced_die_name(), get_spec_die(), inliners_search_cb (+3 more)

### Community 12 - "Community 12"
Cohesion: 0.15
Nodes (7): deleter, handle, _empty, _val, SourceFile, _file, split_source_prefixes()

### Community 13 - "Community 13"
Cohesion: 0.12
Nodes (9): TraceResolverLinuxBase, argv0_, exec_path_, TraceResolverLinuxImpl<trace_resolver_tag::backtrace_symbol>, _symbols, TraceResolverLinuxImpl<trace_resolver_tag::libdw>, _dwfl_cb, _dwfl_handle (+1 more)

### Community 14 - "Community 14"
Cohesion: 0.18
Nodes (5): SignalHandling, LONG, reporter_thread_, signal_skip_recs, _stack_content

### Community 15 - "Community 15"
Cohesion: 0.13
Nodes (15): VSDA, VSDA - Calcium - CreateIndicator, VSDA - Calcium - DefineScanRegion *NEW*, VSDA - Calcium - GetImage *NEW*, VSDA - Calcium - GetImageStack *NEW*, VSDA - Calcium - GetRenderStatus *NEW*, VSDA - Calcium - QueueRenderOperation *NEW*, VSDA - Calcium - Setup (+7 more)

### Community 16 - "Community 16"
Cohesion: 0.21
Nodes (10): ErrResponse(), FindPar(), GetParBool(), GetParFloat(), GetParInt(), GetParString(), GetParVecFloat(), GetParVecInt() (+2 more)

### Community 17 - "Community 17"
Cohesion: 0.21
Nodes (8): BidirectionalRpc(), HealthCheckLoop(), HookbackConnect(), IsConnected(), Reconnect(), Stop(), TestConnection(), UpdatePeer()

### Community 18 - "Community 18"
Cohesion: 0.22
Nodes (10): create_boxes(), create_BS_compartments(), create_cylinders(), create_spheres(), init_VSDA(), int, - (bgSimulationID) `SimulationID` ID of simulation to setup the microscope for., scan_EM_1() (+2 more)

### Community 19 - "Community 19"
Cohesion: 0.15
Nodes (12): 1. Correction, 2. Warning, 3. Temporary Ban, 4. Permanent Ban, Attribution, Contributor Covenant Code of Conduct, Enforcement, Enforcement Guidelines (+4 more)

### Community 20 - "Community 20"
Cohesion: 0.15
Nodes (13): Authentication, BS - BulkCreate, BS - Create, Compartments, GetToken, RecordingElectrode - AddNoise   **NEW**, RecordingElectrode - GetRecording    *NEW**, RecordingElectrode - Initialize     **NEW** (+5 more)

### Community 21 - "Community 21"
Cohesion: 0.15
Nodes (13): VSDA, VSDA - Calcium - DefineScanRegion *NEW*, VSDA - Calcium - GetImage *NEW*, VSDA - Calcium - GetImageStack *NEW*, VSDA - Calcium - GetRenderStatus *NEW*, VSDA - Calcium - QueueRenderOperation *NEW*, VSDA - EM - DefineScanRegion, VSDA - EM - GetImage (+5 more)

### Community 22 - "Community 22"
Cohesion: 0.15
Nodes (12): Build Issues, Building on windows fails with syntax errors, Can this library encrypt/decrypt claims?, Can you add claims to a signed token?, code:cpp (auto token = jwt::create()), Frequently Asked Questions, Handling Tokens, How can new keys be generated for my application? (+4 more)

### Community 23 - "Community 23"
Cohesion: 0.18
Nodes (6): cfile_streambuf, buffer, sink, GetFile(), RouteCallback(), streambuf

### Community 24 - "Community 24"
Cohesion: 0.38
Nodes (11): ConnectEVM(), ConnectionManagerEVM(), ConnectionManagerNES(), ConnectNES(), EVMQueryJSON(), NESQueryJSON(), RunVersionCheckEVM(), RunVersionCheckNES() (+3 more)

### Community 25 - "Community 25"
Cohesion: 0.24
Nodes (3): TraceResolverLinuxImpl<trace_resolver_tag::libbfd>, _bfd_loaded, _fobj_bfd_map

### Community 26 - "Community 26"
Cohesion: 0.18
Nodes (11): Simulation, Simulation - BuildMesh, Simulation - Create, Simulation - Get Recording, Simulation - Get Status, Simulation - Load **NEW**, Simulation - Record All, Simulation - Reset (+3 more)

### Community 27 - "Community 27"
Cohesion: 0.20
Nodes (9): BrainGenix-API Agent Session Notes, Branch and state observed, code:bash (cd BrainGenix-API/Tools), code:bash (cd BrainGenix-API/Tools), Follow-ups before merge request, Purpose, Review outcome, Verification performed (+1 more)

### Community 28 - "Community 28"
Cohesion: 0.38
Nodes (3): load_stacktrace(), StackTraceImplHolder, _stacktrace

### Community 29 - "Community 29"
Cohesion: 0.20
Nodes (3): BackwardCpp, ConanFile, TestBackward

### Community 30 - "Community 30"
Cohesion: 0.20
Nodes (9): Admonishing, password, role, rahul, password, role, thomas, password (+1 more)

### Community 31 - "Community 31"
Cohesion: 0.20
Nodes (9): CMake, code:sh (cmake .), code:cmake (find_package(jwt-cpp CONFIG REQUIRED)), code:cmake (include(FetchContent)), Header Only, Installation, Overview, Package Manager (+1 more)

### Community 32 - "Community 32"
Cohesion: 0.20
Nodes (10): BSNeuron - Create **NEW**, Diagnostic, Echo, GetAPIVersion, Neurons, RecordingElectrode - GetRecording **NEW**, RecordingElectrode - SetElectricFieldPotential **NEW**, Routes (+2 more)

### Community 33 - "Community 33"
Cohesion: 0.20
Nodes (9): buildPresets, cmakeMinimumRequired, major, minor, patch, configurePresets, include, testPresets (+1 more)

### Community 34 - "Community 34"
Cohesion: 0.20
Nodes (5): TraceResolverDarwinImpl<trace_resolver_tag::backtrace_symbol>, _symbols, TraceResolverImpl<system_tag::unknown_tag>, TraceResolverImplBase, _demangler

### Community 36 - "Community 36"
Cohesion: 0.25
Nodes (6): Example, staticfunc, test, virtualfunc, SubclassExample, virtualfunc

### Community 38 - "Community 38"
Cohesion: 0.22
Nodes (8): buildPresets, cmakeMinimumRequired, major, minor, patch, configurePresets, include, version

### Community 39 - "Community 39"
Cohesion: 0.36
Nodes (8): abort_abort_I_repeat_abort_abort(), badass_function(), bye_bye_stack(), divide_by_zero(), TEST_ABORT(), TEST_DIVZERO(), TEST_SEGFAULT(), you_shall_not_pass()

### Community 40 - "Community 40"
Cohesion: 0.22
Nodes (8): buildPresets, cmakeMinimumRequired, major, minor, patch, configurePresets, include, version

### Community 41 - "Community 41"
Cohesion: 0.28
Nodes (3): Initialize(), RegisterVSDANode(), SetVSDALeader()

### Community 42 - "Community 42"
Cohesion: 0.25
Nodes (7): BrainGenix-API Mac Silicon Change Log, Changes made, code:bash (cd BrainGenix-API/Tools), Current branch, Intent, Review notes, Verification

### Community 43 - "Community 43"
Cohesion: 0.29
Nodes (4): StackTraceImpl<system_tag::current_tag>, ctx_, machine_type_, thd_

### Community 44 - "Community 44"
Cohesion: 0.29
Nodes (5): unwind(), Unwinder, _depth, _f, _index

### Community 45 - "Community 45"
Cohesion: 0.25
Nodes (7): Apple Silicon macOS, Build and run, code:bash (xcode-select --install), code:bash (cd Tools), code:bash (cd Tools), Generated files, Prerequisites

### Community 46 - "Community 46"
Cohesion: 0.25
Nodes (7): About, bgRenderStatus, Document Format, Enums, Name Of Route, NES Internal API Version 2024.01.14, StatusCode

### Community 47 - "Community 47"
Cohesion: 0.25
Nodes (8): Simulation, Simulation - BuildMesh, Simulation - Create, Simulation - GetRecording, Simulation - GetStatus, Simulation - RecordAll, Simulation - Reset, Simulation - Run For

### Community 48 - "Community 48"
Cohesion: 0.25
Nodes (7): buildPresets, cmakeMinimumRequired, major, minor, patch, configurePresets, version

### Community 49 - "Community 49"
Cohesion: 0.25
Nodes (3): JWTUtil, ISSUER, SECRET_KEY

### Community 50 - "Community 50"
Cohesion: 0.29
Nodes (7): Box - BulkCreate, Box - Create, Cylinder - BulkCreate, Cylinder - Create, Shapes, Sphere - BulkCreate, Sphere - Create

### Community 51 - "Community 51"
Cohesion: 0.29
Nodes (7): Box - BulkCreate, BS - BulkCreate, BS - Create, code:block1 (Cfg = NES.Models.Neurons.BSNeuron.Configuration()), Compartments, NeuralCircuit - Create      **NEW**, Neuron - Create

### Community 53 - "Community 53"
Cohesion: 0.52
Nodes (6): a(), b(), c(), collect_trace(), d(), TEST()

### Community 54 - "Community 54"
Cohesion: 0.29
Nodes (6): type_context_t, has_name, has_type, is_const, is_typedef, text

### Community 55 - "Community 55"
Cohesion: 0.38
Nodes (3): generate_knowledge_graph(), usage(), Tag.sh script

### Community 57 - "Community 57"
Cohesion: 0.33
Nodes (5): TraceResolverImpl<system_tag::windows_tag>, displacement, image_type, max_sym_len, sym

### Community 58 - "Community 58"
Cohesion: 0.60
Nodes (5): build(), do_action(), dotest(), mkbuild(), builds.sh script

### Community 59 - "Community 59"
Cohesion: 0.33
Nodes (5): dependencies, description, homepage, name, version

### Community 60 - "Community 60"
Cohesion: 0.33
Nodes (5): Apple Silicon macOS, Building, code:bash (cd Tools), Running locally (e.g. to debug NES), Using With Let's Encrypt/Certbot

### Community 61 - "Community 61"
Cohesion: 0.33
Nodes (6): ADC - Create, ADC - Get Recorded Data, ADC - Set Sample Rate, DAC - Create, DAC - Set Output List, Tools

### Community 62 - "Community 62"
Cohesion: 0.33
Nodes (5): API Design Spec, bgRenderStatus, bgServiceStatus, bgStatus, Enums

### Community 63 - "Community 63"
Cohesion: 0.33
Nodes (6): code:block1 (Cfg = NES.Models.Neurons.BSNeuron.Configuration()), Connections, NeuralCircuit - Create      **NEW**, Neuron - Create, Receptor - Create, Staple - Create

### Community 64 - "Community 64"
Cohesion: 0.33
Nodes (6): ADC - Create, ADC - Get Recorded Data, ADC - Set Sample Rate, DAC - Create, DAC - Set Output List, Tools

### Community 65 - "Community 65"
Cohesion: 0.33
Nodes (6): Box - Create, Cylinder - BulkCreate, Cylinder - Create, Shapes, Sphere - BulkCreate, Sphere - Create

### Community 66 - "Community 66"
Cohesion: 0.33
Nodes (5): code:cpp (//include "jwt-cpp/traits/author-library/traits.h"), code:cpp (struct my_favorite_json_library_traits {), JSON Traits, Providing your own JSON Traits, Selecting a JSON library

### Community 68 - "Community 68"
Cohesion: 0.53
Nodes (5): end_of_our_journey(), fib(), foo, rec(), TEST()

### Community 72 - "Community 72"
Cohesion: 0.33
Nodes (3): demangler_impl<system_tag::current_tag>, _demangle_buffer, _demangle_buffer_length

### Community 73 - "Community 73"
Cohesion: 0.33
Nodes (3): TraceResolverLinuxImpl<trace_resolver_tag::libdwarf>, _dwarf_loaded, _fobj_dwarf_map

### Community 75 - "Community 75"
Cohesion: 0.40
Nodes (3): get_mod_info, buffer_length, process

### Community 76 - "Community 76"
Cohesion: 0.40
Nodes (4): code:cpp (struct your_algorithm{), code:cpp (auto token = jwt::create()), Custom Signature Algorithms, Signing Tokens

### Community 77 - "Community 77"
Cohesion: 0.40
Nodes (4): code:sh (cmake . -DJWT_SSL_LIBRARY:STRING=wolfSSL), Cryptography Libraries, Notes, Supported Versions

### Community 78 - "Community 78"
Cohesion: 0.70
Nodes (4): AddRoute(), EVMRequest(), NESRequest(), RPCManager()

### Community 80 - "Community 80"
Cohesion: 0.40
Nodes (5): module_data, base_address, image_name, load_size, module_name

### Community 82 - "Community 82"
Cohesion: 0.40
Nodes (4): dependencies, license, name, version-string

### Community 83 - "Community 83"
Cohesion: 0.50
Nodes (4): Diagnostic, Hello, Status, Version

### Community 84 - "Community 84"
Cohesion: 0.50
Nodes (3): API Status Page Design Document, Introduction, Requirements

### Community 89 - "Community 89"
Cohesion: 0.50
Nodes (3): length, name, padding

### Community 90 - "Community 90"
Cohesion: 0.50
Nodes (4): multitest_entry, expected_ec, fail_bitmask, fail_mask_ptr

### Community 92 - "Community 92"
Cohesion: 0.67
Nodes (3): Connections, Receptor - Create, Staple - Create

### Community 93 - "Community 93"
Cohesion: 0.67
Nodes (3): RecordingElectrode, RecordingElectrode - AddNoise   **NEW**, RecordingElectrode - Initialize **NEW**

## Knowledge Gaps
- **369 isolated node(s):** `name`, `version-string`, `license`, `dependencies`, `overlay-ports` (+364 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **30 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `Printer` connect `Community 7` to `Community 56`, `Community 4`?**
  _High betweenness centrality (0.010) - this node is a cross-community bridge._
- **Why does `SignalHandling` connect `Community 14` to `Community 43`, `Community 4`, `Community 12`?**
  _High betweenness centrality (0.006) - this node is a cross-community bridge._
- **Why does `TraceResolverLinuxImpl<trace_resolver_tag::libdw>` connect `Community 13` to `Community 11`, `Community 4`?**
  _High betweenness centrality (0.004) - this node is a cross-community bridge._
- **What connects `name`, `version-string`, `license` to the rest of the system?**
  _371 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `Community 0` be split into smaller, more focused modules?**
  _Cohesion score 0.041666666666666664 - nodes in this community are weakly interconnected._
- **Should `Community 1` be split into smaller, more focused modules?**
  _Cohesion score 0.047619047619047616 - nodes in this community are weakly interconnected._
- **Should `Community 2` be split into smaller, more focused modules?**
  _Cohesion score 0.05855855855855856 - nodes in this community are weakly interconnected._