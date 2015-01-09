#!/usr/bin/env python2
# -*- coding: utf-8 -*-

from subprocess import PIPE
import subprocess
import shutil
from os import listdir, mkdir
from os.path import isfile, join, realpath, dirname, basename

BASE_DIR = dirname(realpath(__file__))

COMMAND = './build/sc2sim'
BUILDLIST_DIR = join(BASE_DIR, 'lists')
BUILDLIST_EXPECTED_DIR = join(BUILDLIST_DIR, 'expected_stdout')
BUILDLISTS_ACTUAL_DIR = join(BUILDLIST_DIR, 'actual_stdout')


def diff(expected, actual):
    p = subprocess.Popen(['diff', '--side-by-side', '--suppress-common-lines', expected, actual], stdout=PIPE)
    out = p.communicate()[0]

    if out:
        print('-- EXPECTED --            {}            -- ACTUAL --'.format(basename(expected)))
        print(out)


def main():
    # Get all lists from lists/
    all_build_lists = [f for f in listdir(BUILDLIST_DIR) if isfile(join(BUILDLIST_DIR, f)) and f.endswith('.txt')]

    # Build clean output directory
    mkdir(BUILDLISTS_ACTUAL_DIR)

    for build_list in all_build_lists:
        # Launch sc2sim with every build list and capture stdout
        build_list_path = join(BUILDLIST_DIR, build_list)
        sc2sim = subprocess.Popen([COMMAND, build_list_path], stdout=PIPE)
        out = sc2sim.communicate()[0]

        # Compare actual with expected stdout
        actual_stdout_file = join(BUILDLISTS_ACTUAL_DIR, build_list)

        with open(actual_stdout_file, mode='w') as stdout_file:
            stdout_file.writelines(out)
        diff(join(BUILDLIST_EXPECTED_DIR, build_list), actual_stdout_file)

    # Cleanup output directory
    shutil.rmtree(BUILDLISTS_ACTUAL_DIR)


if __name__ == '__main__':
    main()
