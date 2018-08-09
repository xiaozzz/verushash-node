import Options
from os import unlink, symlink, popen
from os.path import exists 

srcdir = "."
blddir = "build"
VERSION = "0.0.5"

def set_options(opt):
  opt.tool_options("compiler_cxx")
  opt.tool_options("compiler_cc")

def configure(conf):
  conf.check_tool("compiler_cxx")
  conf.check_tool("compiler_cc")
  conf.check_tool("node_addon")

  conf.check(lib='ssl', libpath=['/usr/lib', '/usr/local/lib'])

def build(bld):
  obj = bld.new_task_gen("cxx", "shlib", "node_addon")
  obj.target = "verushash"
  obj.include = "common.h"
  obj.include = "haraka.h"
  obj.include = "haraka_portable.h"
  obj.include = "verus_hash.h"
  obj.source = "haraka.c"
  obj.source = "haraka_portable.c"
  obj.source = "verus_hash.cpp"
  obj.source = "verushash.cc"

def shutdown():
  # HACK to get crypto.node out of build directory.
  # better way to do this?
  if Options.commands['clean']:
    if exists('verushash.node'): unlink('verushash.node')
  else:
    if exists('build/default/verushash.node') and not exists('verushash.node'):
      symlink('build/default/verushash.node', 'verushash.node')
