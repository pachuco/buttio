#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "buttio.h"

IOHandler g_ioHand = {0};

static BYTE __inbyte(USHORT port) {
    BYTE data = 0;
    if (!buttio_ru8(&g_ioHand, port, &data)) {
        printf("oops ru8 h%X\n", port);
    }
    return data;
}
static void __outbyte(USHORT port, BYTE data) {
    if(!buttio_wu8(&g_ioHand, port, data)) {
        printf("oops wu8 h%X h%X\n", port, data);
    }
}

static VOID sound(USHORT frequency) {
    USHORT scale;

    if (frequency == 0) {
        __outbyte(0x61, __inbyte(0x61) & ~3);
        return;
    }

    scale = 1193182 / frequency;
    __outbyte(0x43, 0xB6);
    __outbyte(0x42, scale & 0xFF);
    __outbyte(0x42, scale >> 8);
    __outbyte(0x61, __inbyte(0x61) | 3);
}
 
int main() {
    if (buttio_init(&g_ioHand, NULL, BUTTIO_MET_IOPM)) {
        IOPM_ALLOWIO(g_ioHand.iopm, 0x42);
        IOPM_ALLOWIO(g_ioHand.iopm, 0x43);
        IOPM_ALLOWIO(g_ioHand.iopm, 0x61);
        //iopm_fillRange(g_ioHand.iopm, 0x42, 0x43, TRUE);
        //iopm_fillRange(g_ioHand.iopm, 0x61, 0x61, TRUE);
        buttio_flushIOPMChanges(&g_ioHand);
        
        assert(!IOPM_ISDENIED(g_ioHand.iopm, 0x42));
        assert(!IOPM_ISDENIED(g_ioHand.iopm, 0x43));
        assert(!IOPM_ISDENIED(g_ioHand.iopm, 0x61));
        
        sound(1000);
        SleepEx(2000, 1);
        sound(0);
        
        buttio_shutdown(&g_ioHand);
        
        return 0;
    } else {
        printf("Could not init!\n");
        SleepEx(2000, 1);
    }
    return 1;
}
 