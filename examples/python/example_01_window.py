#!/usr/bin/env python

import renderer as rdr

DEFAULT_WIDTH = 1024
DEFAULT_HEIGHT = 768

def main():
    properties = rdr.WindowProperties()
    properties.backend = rdr.WindowBackend.TYPE_GLFW
    properties.width = DEFAULT_WIDTH
    properties.height = DEFAULT_HEIGHT
    properties.title = "Example 01 - Window"

    window = rdr.Window(properties)
    window.RegisterKeyboardCallback(lambda key, action, mods:
        print("key: {}, action: {}. mods: {}".format(key, action, mods)))

    while window.active:
        window.Begin()
        # opengl-stuff can go here :)
        window.End()

if __name__ == '__main__':
    main()