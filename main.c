#include <lib.h>

typedef struct 
{
    union
    {
       unsigned char bytes[4]; 
       unsigned int shorts[2]; 
    };
} Crc32;

typedef struct
{
    Crc32 CRC;
    const char* Name;
} RomFile;


extern Crc32 CRC;

RomFile KnownRoms[]=
{
    { 0xf1,0x87,0x10,0xb4,	"basic10.rom (Oric 1)" },
    { 0xc3,0xa9,0x2b,0xef,	"basic11b.rom (Atmos)" },
    { 0x1d,0x96,0xab,0x50,	"hyperbas.rom (Telestrat)" },
    { 0xB9,0x9A,0x93,0x4B,	"Atmos ROM 1.1 (LOCI patch)" },
    { 0xB5,0xD3,0x12,0xB9,	"Atmos ROM 1.1 (EREBUS patch)" },
    { 0x49,0xa7,0x4c,0x06,	"8dos.rom" },
    { 0x58,0x07,0x95,0x02,	"pravetzt.rom" },
    { 0x2B,0xCD,0xD8,0x7E,	"PRAVETZ (ORICUTRON)" },             // Strangely, does not match any CRC32 of any of the files in the ROMS folder Oo?
    { 0x68,0xb0,0xfd,0xe6,	"teleass.rom" },
    { 0xaa,0x72,0x7c,0x5d,	"telmon24.rom" },
    { 0xbc,0x6f,0x21,0xdb,	"pravetzt-1.0.rom" },
    { 0,0,0,0,0 },
};
/* Additional hashes From HashMyFiles
---- 16K ---- 
---- 8K ---- 
0c82f636		eprom8d.rom			
3e652e2a		cumana.rom			
94358dc6		telmatic.rom			
a9664a9c		microdis.rom			

---- 2K ---- 
37220e89		jasmin.rom			
5d301b9b		8dos2.rom			

---- 1K ---- 
61952e34		bd500.rom			

*/


void PrintWaitMessage(const char* message)
{
    char* StatusScreenLine=(char*)0xbb80+40*27;
    // Erase the existing message
    StatusScreenLine[0]=16+4;
    StatusScreenLine[1]=3;
    memset(StatusScreenLine+2,32,40-2);
    if (message && message[0])
    {
        // Print the new message
        int length=strlen(message);
        strcpy(StatusScreenLine+20-length/2,message);
    }
}

int main()
{
    char* ScreenLine=(char*)0xbb80;          // maps to 0x3b80 on 16KB machines

    memset(ScreenLine,32,40*28);
    sprintf(ScreenLine,"%c%c%c%s%c",16+4,3,10,"         SysInforic 0.1",0);
    ScreenLine+=40;
    sprintf(ScreenLine,"%c%c%c%s%c",16+4,3,10,"         SysInforic 0.1",0);
    ScreenLine+=40;

    // Find the amount of RAM
    {
        unsigned char ram=48;
        if (strcmp((char*)0xbb80,(char*)0x3b80)==0)
        {
            // 16k machine
            ram=16;
        }
        ScreenLine+=40;
        sprintf(ScreenLine,"RAM: %dKB",ram);
        ScreenLine+=40;
    }

    // Find the ROM type
    {
        PrintWaitMessage("Computing CRC32");
        ComputeROMCRC32();
        PrintWaitMessage("");
        ScreenLine+=40;
        sprintf(ScreenLine,"ROM CRC32: %x%x",CRC.shorts[1],CRC.shorts[0]);
        ScreenLine+=40;

        // Search matching rom
        {
            RomFile* romPtr=KnownRoms;
            while (romPtr->Name)
            {
                if ( (romPtr->CRC.bytes[0] == CRC.bytes[3]) && 
                     (romPtr->CRC.bytes[1] == CRC.bytes[2]) && 
                     (romPtr->CRC.bytes[2] == CRC.bytes[1]) && 
                     (romPtr->CRC.bytes[3] == CRC.bytes[0]) )
                {
                    // We have a match
                    break;
                }
                ++romPtr;
            }
            if (romPtr->Name)
            {
                sprintf(ScreenLine,"Match: %s", romPtr->Name);
            }
            else
            {
                sprintf(ScreenLine,"No known ROM found, please contact us!");
            }
            ScreenLine+=40;
        }
    }

    return 0;
}

