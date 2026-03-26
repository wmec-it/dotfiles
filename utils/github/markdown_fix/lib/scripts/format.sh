#!/usr/bin/env bash

# __WMECDOTFILES_f_prettier_pnpm() {

# }

# __WMECDOTFILES_f_prettier_npm() {

# }

__WMECDOTFILES_f_prettier_npx() {
    local ___list_args
    ___list_args=(
        "--list-different"
        "--config"
    )
    local ___command
    ___command="$(npx prettier . --list-different >&1 2>&1)"
    if [[ "${PIPESTATUS[0]}" -eq 0 ]]; then
        printf '%s\n' "${___command}"
        return 0
    else
        return 1
    fi
    ___command="$(npx prettier . --write >&1 2>&1)"
    if [[ "${PIPESTATUS[0]}" -eq 0 ]]; then
        printf '%s\n' "${___command}"
        return 0
    else
        return 1
    fi
}

__WMECDOTFILES_f_handle_package_manager() {
    ___check() {
        local ___input
        ___input="${1:-}"
        if command -v "${___input}" >/dev/null 2>&1; then
            return 0
        else
            return 1
        fi
    }
    if ___check npx; then
        __WMECDOTFILES_f_prettier_npx || return 1
    else
        printf '%s' "Couldn't format, since pnpm, npm, and npx commands do not work/are not installed."
        return 1
    fi
}

__WMECDOTFILES_f_handle_package_manager || exit 1
