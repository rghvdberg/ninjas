#! /usr/bin/env python

import os
import glob
import numpy as np
from tempfile import mkstemp

DEFAULT_SOUND = '22050Hz_5s_brownnoise.wav'

def array_from_text_file(filename, dtype = 'float'):
    filename = os.path.join(os.path.dirname(__file__), filename)
    with open(filename) as f:
        lines = f.readlines()
    return np.array([line.split() for line in lines],
            dtype = dtype)

def list_all_sounds(rel_dir):
    datadir = os.path.join(os.path.dirname(__file__), rel_dir)
    return glob.glob(os.path.join(datadir,'*.*'))

def get_default_test_sound(TestCase, rel_dir = 'sounds'):
    all_sounds = list_all_sounds(rel_dir)
    if len(all_sounds) == 0:
        TestCase.skipTest("please add some sounds in \'python/tests/sounds\'")
    else:
        default_sound = all_sounds[0]
        if DEFAULT_SOUND in map(os.path.basename, all_sounds):
            while os.path.basename(default_sound) != DEFAULT_SOUND:
                default_sound = all_sounds.pop(0)
        return default_sound

def get_tmp_sink_path():
    fd, path = mkstemp()
    os.close(fd)
    return path

def del_tmp_sink_path(path):
    try:
        os.unlink(path)
    except WindowsError as e:
        print("deleting {:s} failed ({:s}), reopening".format(path, repr(e)))
        with open(path, 'wb') as f:
            f.close()
        try:
            os.unlink(path)
        except WindowsError as f:
            print("deleting {:s} failed ({:s}), aborting".format(path, repr(e)))

def array_from_yaml_file(filename):
    import yaml
    f = open(filename)
    yaml_data = yaml.safe_load(f)
    f.close()
    return yaml_data

def count_samples_in_file(file_path):
    from aubio import source
    hopsize = 256
    s = source(file_path, 0, hopsize)
    total_frames = 0
    while True:
        _, read = s()
        total_frames += read
        if read < hopsize: break
    return total_frames

def count_samples_in_directory(samples_dir):
    total_frames = 0
    for f in os.walk(samples_dir):
        if len(f[2]):
            for each in f[2]:
                file_path = os.path.join(f[0], each)
                if file_path:
                    total_frames += count_samples_in_file(file_path)
    return total_frames

def count_files_in_directory(samples_dir):
    total_files = 0
    for f in os.walk(samples_dir):
        if len(f[2]):
            for each in f[2]:
                file_path = os.path.join(f[0], each)
                if file_path:
                    total_files += 1
    return total_files
