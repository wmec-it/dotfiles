#!/usr/bin/env bash

# Definitions to make things more granular
declare    -Ax    WMECDOTFILES_v_runtime
declare    -Ax    WMECDOTFILES_v_config
declare           __WMECDOTFILES_v_input_type

# Assignments
WMECDOTFILES_v_runtime["shell.args"]="${1:-}"
WMECDOTFILES_v_runtime["shell.definitions"]="$(declare -f)"

__WMECDOTFILES_f_config_value() {
    # TODO: Implement error validation and messages
    local ___input
    ___input="${*:-""}"
    [[ -n "${___input}" ]] && printf '%s' "${*:-""}" || return 1
    return 0
}

__WMECDOTFILES_f_set_config() {
    # TODO: Implement error validation and messages
    if [[ -n "${1}" ]]; then
        local ___key
        ___key="${1}"
    else
        return 1
    fi
    shift 1
    if [[ -n "${*}" ]]; then
        local ___value
        ___value="${*}"
    fi
    WMECDOTFILES_v_config["${___key}"]="$(
        __WMECDOTFILES_f_config_value "${___value:-""}"
    )"
    return 0
}

# Config definitions
__WMECDOTFILES_f_set_config  "self.file_name"                                      "${0:-"dev.sh"}"
__WMECDOTFILES_f_set_config  "workspace.path"                                      "$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"
__WMECDOTFILES_f_set_config  "workspace.lib.path"                                  "${WMECDOTFILES_v_config["workspace.path"]}/lib"
__WMECDOTFILES_f_set_config  "workspace.lib.scripts.path"                          "${WMECDOTFILES_v_config["workspace.lib.path"]}/scripts"
__WMECDOTFILES_f_set_config  "workspace.lib.scripts.utils.path"                    "${WMECDOTFILES_v_config["workspace.lib.scripts.path"]}/utils"
__WMECDOTFILES_f_set_config  "workspace.lib.scripts.utils.source.path"             "${WMECDOTFILES_v_config["workspace.lib.scripts.utils.path"]}/source"
__WMECDOTFILES_f_set_config  "lib.scripts.utils.source.spacing.path"               "${WMECDOTFILES_v_config["workspace.lib.scripts.utils.source.path"]}/spacing.sh"
__WMECDOTFILES_f_set_config  "ui.padding"                                          "  "
__WMECDOTFILES_f_set_config  "ui.indent"                                           "    "
__WMECDOTFILES_f_set_config  "ui.indepad"                                          "${WMECDOTFILES_v_config["ui.indent"]}${WMECDOTFILES_v_config["ui.padding"]}"

__WMECDOTFILES_f_usage() {
    local ___usage
    ___usage=(
        "Usage: dev.sh [options]"
        ""
        "Execution Options:"
        ""
        "${WMECDOTFILES_v_config["ui.indepad"]}format"
        "${WMECDOTFILES_v_config["ui.indepad"]}${WMECDOTFILES_v_config["ui.indepad"]}${WMECDOTFILES_v_config["ui.indepad"]}"
    )
    printf '\n%b\n' ""
}

case "${__WMECDOTFILES_v_input_type}" in
    format)
        __WMECDOTFILES_f_format
        ;;
    *)
        __WMECDOTFILES_f_usage
        exit 0
        ;;
esac

eval "${WMECDOTFILES_v_runtime["shell.definitions"]}"
