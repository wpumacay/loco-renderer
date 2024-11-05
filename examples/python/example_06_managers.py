#!/usr/bin/env python

from pathlib import Path
from typing import Optional

import numpy as np
from OpenGL.GL import *  # type: ignore

import renderer as rdr
from renderer.opengl import (
    OpenGLBufferElement,
    OpenGLBufferLayout,
    OpenGLProgram,
    OpenGLVertexArray,
    OpenGLVertexBuffer,
    OpenGLIndexBuffer,
    ResourcesManager,
)


class Engine:
    def __init__(self):
        self.window: Optional[rdr.Window] = None
        self.resources_manager: Optional[ResourcesManager] = None
        self.input_manager: Optional[rdr.InputManager] = None


g_engine = Engine()


def main() -> int:
    global g_engine
    win_config = rdr.WindowConfig()
    win_config.backend = rdr.WindowBackend.TYPE_GLFW
    win_config.width = 1024
    win_config.height = 768
    win_config.title = "Example 06 - Managers"
    win_config.gl_version_major = 3
    win_config.gl_version_minor = 3

    g_engine.window = rdr.Window.CreateWindow(win_config)
    assert g_engine.window is not None

    def keyboard_callback(key: int, action: int, _: int) -> None:
        if g_engine.input_manager is not None:
            g_engine.input_manager.CallbackKey(key, action)

    g_engine.window.RegisterKeyboardCallback(keyboard_callback)

    def mouse_button_callback(
        button: int, action: int, x: float, y: float
    ) -> None:
        if g_engine.input_manager is not None:
            g_engine.input_manager.CallbackMouseButton(button, action, x, y)

    g_engine.window.RegisterMouseButtonCallback(mouse_button_callback)

    def mouse_move_callback(x: float, y: float) -> None:
        if g_engine.input_manager is not None:
            g_engine.input_manager.CallbackMouseMove(x, y)

    g_engine.window.RegisterMouseMoveCallback(mouse_move_callback)

    def scroll_callback(xOff: float, yOff: float) -> None:
        if g_engine.input_manager is not None:
            g_engine.input_manager.CallbackScroll(xOff, yOff)

    g_engine.window.RegisterScrollCallback(scroll_callback)

    g_engine.resources_manager = ResourcesManager()
    g_engine.input_manager = rdr.InputManager()

    assert g_engine.resources_manager is not None
    assert g_engine.input_manager is not None

    SHADERS_FOLDER = Path(__file__).parent.parent / "resources" / "shaders"
    VERT_SHADER_FILE = SHADERS_FOLDER / "basic2d_vert.glsl"
    FRAG_SHADER_FILE = SHADERS_FOLDER / "basic2d_frag.glsl"

    program = g_engine.resources_manager.LoadProgram(
        "basic2d",
        str(VERT_SHADER_FILE.resolve()),
        str(FRAG_SHADER_FILE.resolve()),
    )
    program.Build()
    if not program.valid:
        print("There was an error building the shader program")
        return 1

    IMAGES_FOLDER = Path(__file__).parent.parent / "resources" / "images"
    TEXTURE_FILE = IMAGES_FOLDER / "container.jpg"
    texture = g_engine.resources_manager.LoadTexture(
        "container", str(TEXTURE_FILE.resolve())
    )

    # fmt: off
    buffer_data = np.array([
        # |--pos--|-texture-|#
        -0.5, -0.5, 0.0, 0.0,  # noqa: E131
         0.5, -0.5, 2.0, 0.0,  # noqa: E131
         0.5,  0.5, 2.0, 2.0,  # noqa: E131
        -0.5,  0.5, 0.0, 2.0,  # noqa: E131
    ], dtype=np.float32)

    NUM_VERTICES = 6

    buffer_indices = np.array([
        0, 1, 2,
        0, 2, 3
    ], dtype=np.uint32)
    # fmt: on

    layout = OpenGLBufferLayout()
    layout.AddElement(
        OpenGLBufferElement("position", rdr.ElementType.FLOAT_2, False)
    )
    layout.AddElement(
        OpenGLBufferElement("texcoord", rdr.ElementType.FLOAT_2, False)
    )

    vbo = OpenGLVertexBuffer(layout, rdr.BufferUsage.STATIC, buffer_data)
    ibo = OpenGLIndexBuffer(rdr.BufferUsage.STATIC, buffer_indices)

    vao = OpenGLVertexArray()
    vao.AddVertexBuffer(vbo)
    vao.SetIndexBuffer(ibo)

    while g_engine.window.active:
        g_engine.window.Begin()

        if g_engine.input_manager.IsKeyDown(rdr.Keys.KEY_ESCAPE):
            g_engine.window.RequestClose()

        cursor_position = g_engine.input_manager.GetCursorPosition()
        print(f"CursorPosition: ({cursor_position.x}, {cursor_position.y})")
        print(
            "MouseButtonLeft: {}".format(
                g_engine.input_manager.IsMouseDown(rdr.Mouse.BUTTON_LEFT)
            )
        )
        print(
            "MouseButtonMiddle: {}".format(
                g_engine.input_manager.IsMouseDown(rdr.Mouse.BUTTON_MIDDLE)
            )
        )
        print(
            "MouseButtonRight: {}".format(
                g_engine.input_manager.IsMouseDown(rdr.Mouse.BUTTON_RIGHT)
            )
        )

        program.Bind()
        texture.Bind()
        vao.Bind()

        glDrawElements(GL_TRIANGLES, NUM_VERTICES, GL_UNSIGNED_INT, None)

        vao.Unbind()
        texture.Unbind()
        program.Unbind()

        g_engine.window.End()

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
