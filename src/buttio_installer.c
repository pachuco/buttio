#include <windows.h>
#include <winioctl.h>
#include <stdio.h>

#include "buttio.h"


void uninstallService(SC_HANDLE hService) {
    if (hService) {
        SERVICE_STATUS status;
        
        ControlService(hService, SERVICE_CONTROL_STOP, &status);
        DeleteService(hService);
        CloseServiceHandle(hService);
    }
}

void doDriverStuff(BOOL isInstall) {
    SC_HANDLE hManager;
    SC_HANDLE hService;
    
    hManager = OpenSCManager(NULL, NULL, GENERIC_ALL);
    
    if (hManager) {
        hService = OpenService(hManager, DRIVER_NAME, SERVICE_ALL_ACCESS);
        uninstallService(hService);
        
        if (isInstall) {
            CHAR driverPath[MAX_PATH];
            driverPath[MAX_PATH-1] = '\0';
            
            GetSystemDirectoryA(driverPath, MAX_PATH-1);
            strncat(driverPath, DRIVER_NAME".sys", MAX_PATH-1);
            
            CopyFileA(DRIVER_NAME".sys", driverPath, FALSE);
            
            
            hService = CreateService(hManager, DRIVER_NAME, DRIVER_NAME,
                    SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
                    driverPath, NULL, NULL, NULL, NULL, NULL);
        }
        
        CloseServiceHandle(hManager);
    }
}





void printUsage() {
    printf(
        "Usage: buttioinst.exe -PARAM\n"
        "   -i: install/update with buttio.sys from current dir\n"
        "   -u: uninstalls driver\n"
    );
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printUsage();
        return 1;
    }
    
    #define C(X) (!strcmp(argv[1], X) && strlen(argv[1]) == sizeof(X)-1)
    if        (C("-i")) {
        doDriverStuff(TRUE);
    } else if (C("-u")) {
        doDriverStuff(FALSE);
    }
    #undef C
    
    return 0;
}