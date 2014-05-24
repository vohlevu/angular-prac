{
  "targets": [
    {
      "target_name": "controller",
      "sources": [ "controller.cpp",
					"dtvcontroller.cpp",
					"./src/database.cpp",
					"./src/zenotimer.cpp",
					"./src/dtvutility.cpp",
					"./src/sysinfo.cpp",
					"./src/network/NetworkServices.cpp",
					"./src/network/ServerNetwork.cpp",
					"./src/network/ServerSocket.cpp",
					"./src/connect_request.cpp" ],
		"include_dirs": [
			"include"],
		"libraries": [
			"-l../libs/json_vc10_libmt.lib",
			"-l../libs/libcurl.lib",
			"-l../libs/sqlite3.lib"]
    }
  ]
}
