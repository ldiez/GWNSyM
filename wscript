#!/usr/bin/env python
import os
from subprocess import call
from multiprocessing import Process
import time
import shutil
import ntpath

APPNAME = 'waf-project'
VERSION = '1.0'

top = '.' 
out = 'build'

#
# Init function. It gives support to debug-release building
#
def init (ctx) : 
	from waflib.Build import BuildContext, CleanContext, InstallContext, UninstallContext

	for x in 'debug release'.split() : 
		for y in (BuildContext, CleanContext, InstallContext, UninstallContext) : 
			name = y.__name__.replace('Context','').lower()
			class tmp(y):
				cmd = name + '_' + x
				variant = x

def options(ctx) :
	ctx.load('compiler_cc compiler_cxx')
	ctx.add_option('-b', help=(''), type="string", default='', dest='build')
	ctx.add_option('-r', help=(''), type="string", default='', dest='run')

def configure (ctx) :
	ctx.setenv('release')
	ctx.env.LIBPATH_MYLIB = ['/usr/local/lib', '/usr/lib']
	ctx.env.INCLUDES_MYLIB  = ['/usr/local/include', '/usr/include']
	ctx.env.append_value('LINKFLAGS', ['-ldl', '-lpthread', '-lboost_filesystem',\
                                       '-lboost_system', '-lpython2.7'])
	ctx.load('compiler_cc compiler_cxx')
	ctx.env.CCFLAGS = ['-Os', '-Werror', '-std=c1y']
	ctx.env.CXXFLAGS = ['-Os', '-Werror', '-std=c++1y']
	ctx.env.log = False;

		
	ctx.setenv ('debug')
	ctx.env.LIBPATH_MYLIB = ['/usr/local/lib', '/usr/lib']
	ctx.env.INCLUDES_MYLIB  = ['/usr/local/include', '/usr/include']
	ctx.env.append_value('LINKFLAGS', ['-ldl', '-lpthread', '-lboost_filesystem',\
                                       '-lboost_system', '-lpython2.7'])
	ctx.load('compiler_cc compiler_cxx')
	ctx.env.CCFLAGS = ['-g', '-Wall', '-std=c1y']
	ctx.env.CXXFLAGS = ['-g', '-Wall', '-std=c++1y']
	ctx.env.log = True;
	
	

def build (ctx) : 
	## core module is to be built and will be used in the scenario
	ctx.recurse('core')
	modules = ['system']		

	x = os.walk('modules').next()[1]
	for module in x :
		## apply build recursively to all the modules
		ctx.recurse('modules/' + module)
		## all modules could be used
		modules.append(module)
	
	# Enable/disable logging	
	if ctx.env.log :
		log_define = ['LOG_ENABLED']
		print "enabling log in binary"
		mode = 'debug'
	else :
		log_define = []
		mode = 'release'
	if ctx.options.build == '':
	 	scenario =  ctx.path.ant_glob('scenarios/*.cc, scenarios/*.cpp, scenarios/*.c')
		scenarios = os.walk('scenarios').next()[2]
		for scenario in scenarios :
			if scenario.endswith('.c') or scenario.endswith('.cpp') or scenario.endswith('.cc') :
				build_program (ctx, scenario, mode, log_define, modules)
	else : # specified scenario name
		build_program (ctx, ntpath.split(ctx.options.build)[1], mode, log_define, modules)

def build_program (ctx, file_name, mode, log_define, modules) :
	ctx.program (
			name = os.path.splitext(file_name)[0],
			target = '../../scenarios/' + mode + '/' + os.path.splitext(file_name)[0],
			features = ['cprogram', 'cxxprogram'],
			rpath = ['./build/%s' % mode, './build/%s/modules' % mode],
			defines = log_define,
			source = 'scenarios/' + file_name,
			use = modules
		)

def distclean (ctx):
	"""
	Cleans up more thoroughly (by deleting the build directory), 
	requiring configure again.
	"""
	try:
		shutil.rmtree('build')
	except OSError, e:
		print ("INFO: %s - %s." % (e.filename,e.strerror))
	try:
		shutil.rmtree('scenarios/debug')
	except OSError, e:
		print ("INFO: %s - %s." % (e.filename,e.strerror))
	try:		
		shutil.rmtree('scenarios/release')
	except OSError, e:
		print ("INFO: %s - %s." % (e.filename,e.strerror))

def dist (ctx):
	print("dist!!")

