#include "ethrlp.h"

//-----------------------------------------------------------------------
int main(int argc, const char *argv[])
{
        CRLPOptions options;
        if (!options.prepareArguments(argc, argv))
                return 0;

        while (!options.commandList.IsEmpty())
        {
//XX_B(options.hex2Ascii);
//XX_B(options.ascii2Hex);
//XX_A(options.input);
//XX_A(options.commandList);

                SFString command = nextTokenClear(options.commandList, '\n');
                options.parseArguments(command);
		if (options.ascii2Hex)
		{
			out << string2Hex(options.input);

		} else
		{
			out << hex2String(options.input);
		}
        }
        return 0;
}

CFileExportContext out;
