# EthSlurp - Installation and Build Instructions

Follow these instructions to compile and use our software.

    1. Open a command prompt or terminal window on your system.
    2. Change to the folder containing this file.
    3. From the command prompt enter 'make clean' then 'make'.

It should be as simple as that.

### Running EthSlurp

To run the software, navigate to the same folder as above and enter:

    ./ethslurp --version

If the version string appears, the software is ready to go. At any time you may enter --help to see the program's help
screen. If you wish, you may copy or move the executable to any folder in your $PATH.

### Building the Software from Source

Currently we have no binaries to download, therefore you must build from source.

Start with this command line command:

    git clone http://github.com/Great-Hill-Corporation/ethslurp

Then

    cd ethslurp

We think there is only a single dependency in our code for the 'curl' libraries.

On Ubuntu, run this command to install 'curl' on your system:

    apt-get install libcurl4-gnutls-dev

Once 'curl' is installed, you must edit the makefile so that the build finds the curl library. Please see this comment for more information on linking to 'curl' on your system: https://github.com/Great-Hill-Corporation/ethslurp/issues/87#issuecomment-256775010

On the Mac, this link may help you get started:

    http://macappstore.org/curl/

Good luck.
