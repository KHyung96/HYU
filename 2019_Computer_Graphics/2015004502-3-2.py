import numpy as np
import glfw
from OpenGL.GL import *

T = np.array([[1., 0., 0.],
              [0., 1., 0.],
              [0., 0., 1.]])

def render(T):
    glClear(GL_COLOR_BUFFER_BIT)
    glLoadIdentity()
    # draw cooridnate
    glBegin(GL_LINES)
    glColor3ub(255, 0, 0)
    glVertex2fv(np.array([0.,0.]))
    glVertex2fv(np.array([1.,0.]))
    glColor3ub(0, 255, 0)
    glVertex2fv(np.array([0.,0.]))
    glVertex2fv(np.array([0.,1.]));
    glEnd()
    # draw triangle
    glBegin(GL_TRIANGLES)
    glColor3ub(255, 255, 255)
    glVertex2fv( (T @ np.array([.0,.5,1.]))[:-1] )
    glVertex2fv( (T @ np.array([.0,.0,1.]))[:-1] )
    glVertex2fv( (T @ np.array([.5,.0,1.]))[:-1] )
    glEnd()

def key_callback(window, key, scancode, action, mods):
    global T
    if key == glfw.KEY_W and action == glfw.PRESS:
        T = np.array([[0.9, 0., 0.],
                      [0., 1., 0.],
                      [0., 0., 1.]]) @ T
    elif key == glfw.KEY_E and action == glfw.PRESS:
        T = np.array([[1.1, 0., 0.],
                      [0., 1., 0.],
                      [0., 0., 1.]]) @ T
    elif key == glfw.KEY_S and action == glfw.PRESS:
        T = np.array([[np.cos(np.pi/18), -np.sin(np.pi/18), 0.],
                      [np.sin(np.pi/18), np.cos(np.pi/18), 0.],
                      [0., 0., 1.]]) @ T
    elif key == glfw.KEY_D and action == glfw.PRESS:
        T = np.array([[np.cos(np.pi/18), np.sin(np.pi/18), 0.],
                      [-np.sin(np.pi/18), np.cos(np.pi/18), 0.],
                      [0., 0., 1.]]) @ T
    elif key == glfw.KEY_X and action == glfw.PRESS:
        T = np.array([[1., -0.1, 0.],
                      [0., 1., 0.],
                      [0., 0., 1.]]) @ T
    elif key == glfw.KEY_C and action == glfw.PRESS:
        T = np.array([[1., 0.1, 0.],
                      [0., 1., 0.],
                      [0., 0., 1.]]) @ T
    elif key == glfw.KEY_R and action == glfw.PRESS:
        T = np.array([[1., 0., 0.],
                      [0., -1., 0.],
                      [0., 0., 1.]]) @ T
    elif key == glfw.KEY_1 and action == glfw.PRESS:
        T = np.array([[1., 0., 0.],
                      [0., 1., 0.],
                      [0., 0., 1.]])

def main():
    
    if not glfw.init():
        return
    
    window = glfw.create_window(480, 480, "2015004502-3-2", None, None)
    if not window:
        glfw.terminate()
        return

    glfw.set_key_callback(window, key_callback)

    glfw.make_context_current(window)

    while not glfw.window_should_close(window):
        glfw.poll_events()
        
        global T
        render(T)
        
        glfw.swap_buffers(window)

    glfw.terminate()

if __name__ == "__main__":
    main()
