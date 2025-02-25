#pragma once

#define EXECUTE_ON_SERVER if (!IsNetMode(NM_Client))
#define EXECUTE_ON_CLIENT if (IsNetMode(NM_Client))