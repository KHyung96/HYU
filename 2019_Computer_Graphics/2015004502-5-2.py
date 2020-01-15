import numpy as np
import glfw
from OpenGL.GL import *
from OpenGL.GLU import *

def drawFrame():
    glBegin(GL_LINES)
    glColor3ub(255, 0, 0)
    glVertex2fv(np.array([0.,0.]))
    glVertex2fv(np.array([1.,0.]))
    glColor3ub(0, 255, 0)
    glVertex2fv(np.array([0.,0.]))
    glVertex2fv(np.array([0.,1.]))
    glEnd()

def drawTriangle():
    glBegin(GL_TRIANGLES)
    glVertex2fv(np.array([0.,.5]))
    glVertex2fv(np.array([0.,0.]))
    glVertex2fv(np.array([.5,0.]))
    glEnd()

def main():
    
    if not glfw.init():
        return
    
    window = glfw.create_window(480, 480, "2015004502-5-2", None, None)
    if not window:
        glfw.terminate()
        return

    glfw.make_context_current(window)

    glClear(GL_COLOR_BUFFER_BIT)

    while not glfw.window_should_close(window):
        glfw.poll_events()
        
        glLoadIdentity()
        drawFrame()
        glColor3ub(255, 255, 255)
        drawTriangle()
        
        glTranslatef(0.6,0.,0.)
        glRotatef(30,0,0,1)
        # Same with
        # M = np.array([[np.cos(np.radians(30)),-np.sin(np.radians(30)),0.,0.6],
        #               [np.sin(np.radians(30)),np.cos(np.radians(30)),0.,0.],
        #               [0.,0.,1.,0.],
        #               [0.,0.,0.,1.]])
        # glMultMatrix(M.T)
        drawFrame()
        glColor3ub(0, 0, 255)
        drawTriangle()
        
        glfw.swap_buffers(window)

    glfw.terminate()

if __name__ == "__main__":
    main()
