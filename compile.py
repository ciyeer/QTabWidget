#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Date    : 2021-11-28 17:45:33
# @Author  : shixiong.Liu (liushixiong@corp.netease.com)

import os
import psutil
import subprocess
import sys
import argparse
import colorama
import json
import asyncio

'''
- √ 配置json数据 配置文件可以指定路径再加载
- √ 编译处理，Debug|Release
- √ 能够杀掉进程，考虑多进程的问题
- √ 资源编译
- √ 增加编译错误检测
- √ 增加编译警告检测
'''

error_list = []
warning_list = []
windows_console_encode = 'gb2312'


async def _read_stream(stream, cb):
    while True:
        line = await stream.readline()
        if line:
            content = str(line, encoding=windows_console_encode).splitlines(False)
            if content[0].find(' error ') != -1:
                error_list.append(content)
            elif content[0].find(' warning ') != -1:
                warning_list.append(content)
            cb(content[0])
        else:
            break


async def _stream_subprocess(cmd, stdout_cb, stderr_cb):
    process = await asyncio.create_subprocess_shell(*cmd,
                                                    stdout=asyncio.subprocess.PIPE,
                                                    stderr=asyncio.subprocess.PIPE)

    await asyncio.wait([
        _read_stream(process.stdout, stdout_cb),
        _read_stream(process.stderr, stderr_cb)
    ])
    return await process.wait()


def execute(cmd, stdout_cb, stderr_cb):
    loop = asyncio.get_event_loop()
    rc = loop.run_until_complete(_stream_subprocess(
                                                    cmd,
                                                    stdout_cb,
                                                    stderr_cb))
    loop.close()
    return rc


class App:
    def __init__(self):
        self._clear()

    def _clear(self):
        self.pre_compile_command = ""
        self.start_process_args = ""
        self.process_name = ""
        self.start_process_path = ""
        self.update_code_command = ""
        self.compile_args = ""
        self.compile_file = ""
        self.compile_tool_dir = ""

        self.update = False
        self.resource = False
        self.is_kill = False
        self.is_start = False
        self.is_compile = False
        self.compile_args = ""
        self.config_path = ""

    def _search_process(self):
        list_process_id = []
        for proc in psutil.process_iter(['name', 'pid']):
            if proc.info['name'] == self.process_name:
                find_flag = 1
                list_process_id.append(proc.info['pid'])
                break
        if len(list_process_id) == 0:
            print(colorama.Fore.GREEN + "Not find target process")
        return list_process_id

    def _kill_process(self, list_process_id):
        for process_id in list_process_id:
            target_process = psutil.Process(process_id)
            try:
                target_process.kill()
                print(colorama.Fore.GREEN + "Kill target process successful.")
            except psutil.ZombieProcess:
                print(colorama.Fore.RED + "Error: this is zombie process")
                return 1
            except psutil.AccessDenied:
                print(colorama.Fore.RED + "Error: Access denied")
                return 1
            except psutil.TimeoutExpired:
                print(colorama.Fore.RED + "Error: Time out expired")
                return 1
        return 0

    def _pre_compile_code(self):
        if len(self.pre_compile_command) == 0:
            print(colorama.Fore.RED + "Error: not any previous compile command.")
            return
        os.system(self.pre_compile_command)
        print(colorama.Fore.GREEN + "Execute previous compile command finished.")
        return

    def _compile_code(self):
        compile_tool = 'devenv'
        if self.rebuild:
            compile_parameter = '/rebuild "' + self.compile_args + '"'
        else:
            compile_parameter = '/build "' + self.compile_args + '"'
        execute(['devenv ' + self.compile_file + ' ' + compile_parameter],
                lambda x: print("%s" % x),
                lambda x: print("%s" % x))
        return len(error_list)

    def _pre_env(self):
        strewn = os.getenv("path")
        os.putenv("path", strewn + ";" + self.compile_tool_dir)
        return

    def _start_process(self):
        try:
            list_process_args = [self.start_process_path]
            for args in self.start_process_args:
                list_process_args.append(args)
            subprocess.Popen(list_process_args, shell=False)
            print(colorama.Fore.GREEN + "Start target process successful.")
        except Exception as error:
            print(colorama.Fore.GREEN + str(error))
        else:
            pass
        return

    def _update_code(self):
        if len(self.update_code_command) == 0:
            print(colorama.Fore.RED + "Error: Not find any update code command.")
        else:
            os.system(self.update_code_command)
        return

    def _parse_args(self, args):
        parser = argparse.ArgumentParser(description='Tools for auto update code and compile project')
        parser.add_argument('-u', '--update', action='store_true', help='update code by svn or git')
        parser.add_argument('-pre', '--precompileaction', action='store_true', help='build code previous action,for '
                                                                                    'example Qt qrc file.')
        parser.add_argument('-k', '--kill', action='store_true', help='kill target process')
        parser.add_argument('-s', '--start', action='store_true', help='start target process')
        parser.add_argument('-c', '--compile', action='store_true', help='compile project')
        parser.add_argument('-a', '--compileargs', default='Debug|Win32', type=str,
                            help='compile project with args, for example:"Debug|Win32" or "Release|Win32". default '
                                 'value: "Debug|Win32"')
        parser.add_argument('-p', '--configpath', default='./config.json', type=str, help='load config json path')
        parser.add_argument('-r', '--rebuild', action='store_true', help='rebuild solution')
        args = parser.parse_args(args)
        self.update = args.update
        self.pre_compileaction = args.precompileaction
        self.is_kill = args.kill
        self.is_start = args.start
        self.is_compile = args.compile
        self.compile_args = args.compileargs
        self.config_path = args.configpath
        self.rebuild = args.rebuild

    def _parse_config(self, config_path):
        with open(config_path) as f:
            data = json.load(f)
        self.process_name = data['process_name']
        self.update_code_command = data['update_code_command']
        self.pre_compile_command = data['pre_compile_command']
        self.compile_file = data['compile_file']
        self.compile_tool_dir = data['compile_tool_dir']
        self.start_process_path = data['start_process_path']
        self.start_process_args = data['start_process_args']

    def run(self, args):
        self._parse_args(args)
        self._parse_config(self.config_path)

        if self.update:
            print('------------------------start update code----------------------------')
            self._update_code()
            print('------------------------end update code------------------------------')
        if self.pre_compileaction:
            print('-------------------start previous action--------------------------')
            self._pre_compile_code()
            print('--------------------end previous action---------------------------')

        kill_flag = 0
        if self.is_kill:
            print('--------------------start search process--------------------------')
            list_process_id = self._search_process()
            if len(list_process_id) != 0:
                kill_flag = self._kill_process(list_process_id)
            print('---------------------end search process---------------------------')
        if kill_flag != 0:
            print('--------------------stop compile project--------------------------')
        else:
            if self.is_compile:
                print('--------------------start compile project--------------------------')

                print('--------------start init previous environment--------------------')
                self._pre_env()
                print('---------------end init previous environment---------------------')

                result_code = self._compile_code()
                if result_code == 0:
                    print(colorama.Fore.GREEN + 'compile project finished')
                else:
                    print(colorama.Fore.RED + 'compile project errored')
                if len(error_list) > 0:
                    for index in error_list:
                        print(colorama.Fore.RED + index[0])
                if len(warning_list) > 0:
                    for index in warning_list:
                        print(colorama.Fore.YELLOW + index[0]) 
                print('--------------------end compile project----------------------------')
                if result_code > 0:
                    return

            if self.is_start:
                print('--------------------start call application-------------------------')
                self._start_process()
                print('--------------------end call application---------------------------')


if __name__ == '__main__':
    colorama.init(autoreset=True)
    app = App()
    if not app.run(sys.argv[1:]):
        sys.exit(-1)
    sys.exit(0)
