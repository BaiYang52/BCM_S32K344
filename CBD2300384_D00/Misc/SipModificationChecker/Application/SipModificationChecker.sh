SCRIPT_DIR=`dirname $0`
SRC_DIR=$SCRIPT_DIR/../..
SIP_CHECKER=$SCRIPT_DIR/SipModificationChecker.exe
REFERENCE_FILE=$SCRIPT_DIR/SipModificationChecker.xml

$SIP_CHECKER $SRC_DIR $REFERENCE_FILE

echo # Adding newline for aesthetic
