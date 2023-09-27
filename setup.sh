SCRIPT_PATH="${BASH_SOURCE:-$0}"
ABS_SCRIPT_PATH="$(realpath "${SCRIPT_PATH}")"
ABS_DIRECTORY="$(dirname "${ABS_SCRIPT_PATH}")"

SOURCEDIR="${ABS_DIRECTORY}""/DemoUEFMIPlugin/Plugins/UEFMI/ThirdParty/fmikit/src/."
DESTDIR="${ABS_DIRECTORY}""/DemoUEFMIPlugin/Plugins/UEFMI/Source/UEFMI/ThirdParty/fmikit/src/"
cp -a "${SOURCEDIR}" "${DESTDIR}"
