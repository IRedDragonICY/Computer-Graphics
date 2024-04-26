def bresenham(x0, y0, x1, y1):
    dx = abs(x1 - x0)
    dy = abs(y1 - y0)
    x, y = x0, y0
    sx = -1 if x0 > x1 else 1
    sy = -1 if y0 > y1 else 1
    if dx > dy:
        pk = 2*dy - dx
        two_dy = 2*dy
        two_dy_dx = 2*(dy - dx)
        while x != x1:
            x += sx
            if pk < 0:
                pk += two_dy
            else:
                y += sy
                pk += two_dy_dx
            yield x, y, pk
    else:
        pk = 2*dx - dy
        two_dx = 2*dx
        two_dx_dy = 2*(dx - dy)
        while y != y1:
            y += sy
            if pk < 0:
                pk += two_dx
            else:
                x += sx
                pk += two_dx_dy
            yield x, y, pk

print("k\tp_k\t(x_k+1, y_k+1)")
x1 = 1
y1 = 2
x2 = 5
y2 = 5
for k, (x, y, pk) in enumerate(bresenham(x1, y1, x2, y2)):
    print(f"{k}\t{pk}\t({x}, {y})")
