# Copyright 2014-present PlatformIO <contact@platformio.org>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import json
import os
import platform

from platformio.managers.platform import PlatformBase


class Maxim32Platform(PlatformBase):

    def is_embedded(self):
        return True

    def configure_default_packages(self, variables, targets):
        if variables.get("board"):
            board = variables.get("board")
            if "mbed" in variables.get("pioframework", []):
                deprecated_boards_file = os.path.join(
                    self.get_dir(), "misc", "mbed_deprecated_boards.json")
                if os.path.isfile(deprecated_boards_file):
                    with open(deprecated_boards_file) as fp:
                        if board in json.load(fp):
                            self.packages["framework-mbed"]["version"] = "~6.51506.0"

            upload_protocol = variables.get("upload_protocol", self.board_config(
                board).get("upload.protocol", ""))
            if upload_protocol == "cmsis-dap":
                self.packages["tool-pyocd"]["type"] = "uploader"

        return PlatformBase.configure_default_packages(self, variables,
                                                       targets)

    def get_boards(self, id_=None):
        result = PlatformBase.get_boards(self, id_)
        if not result:
            return result
        if id_:
            return self._add_default_debug_tools(result)
        else:
            for key, value in result.items():
                result[key] = self._add_default_debug_tools(result[key])
        return result

    def _add_default_debug_tools(self, board):
        debug = board.manifest.get("debug", {})
        upload_protocols = board.manifest.get("upload", {}).get(
            "protocols", [])
        if "tools" not in debug:
            debug["tools"] = {}

        for link in ("jlink", ):
            if link not in upload_protocols or link in debug["tools"]:
                continue

            if link == "jlink":
                assert debug.get("jlink_device"), (
                    "Missed J-Link Device ID for %s" % board.id)
                debug["tools"][link] = {
                    "server": {
                        "package": "tool-jlink",
                        "arguments": [
                            "-singlerun",
                            "-if", "SWD",
                            "-select", "USB",
                            "-device", debug.get("jlink_device"),
                            "-port", "2331"
                        ],
                        "executable": ("JLinkGDBServerCL.exe"
                                       if platform.system() == "Windows" else
                                       "JLinkGDBServer")
                    },
                    "onboard": link in debug.get("onboard_tools", [])
                }

        board.manifest["debug"] = debug
        return board
