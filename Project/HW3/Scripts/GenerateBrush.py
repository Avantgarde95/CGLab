from PIL import Image
import random


def rgb(f1, f2, f3):
    return (int(255 * f1), int(255 * f2), int(255 * f3))


image = Image.new('RGB', (1024, 1024), rgb(1.0, 1.0, 1.0))
w, h = image.size
pixels = image.load()


def drawEllipse(cx, cy, rx, ry, color):
    sx, ex = max(cx - rx, 0), min(cx + rx, w - 1)
    sy, ey = max(cy - ry, 0), min(cy + ry, h - 1)

    for x in range(sx, ex + 1):
        for y in range(sy, ey + 1):
            dx = x - cx
            dy = y - cy

            if dx * dx / float(rx * rx) + dy * dy / float(ry * ry) <= 1:
                pixels[x, y] = color


# Draw random-sized random-positioned ellipses.
for i in range(4000):
    cx = random.randint(0, w - 1)
    cy = random.randint(0, h - 1)
    ry = random.randint(6, 15)
    rx = int(ry * random.uniform(1.5, 6.0))
    level = random.uniform(0.6, 1.0)
    color = rgb(level, level, level)

    drawEllipse(cx, cy, rx, ry, color)

image.save('Brush.png')
