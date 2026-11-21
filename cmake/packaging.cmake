if (WIN32)
	set(CPACK_GENERATOR ZIP)
else()
	set(CPACK_GENERATOR TGZ)
endif()

set(CPACK_PACKAGE_VERSION 0.1.0)
include(CPack)
