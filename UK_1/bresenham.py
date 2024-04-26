def bresenham(x0, y0, x1, y1):
    step_info = []
    step_info.append("Calculate dx and dy:")
    step_info.append("dx = abs(x1 - x0)")
    dx = abs(x1 - x0)
    step_info.append(f"dx = abs({x1} - {x0})")
    step_info.append(f"dx = {dx}")
    step_info.append("dy = abs(y1 - y0)")
    dy = abs(y1 - y0)
    step_info.append(f"dy = abs({y1} - {y0})")
    step_info.append(f"dy = {dy}")
    step_info.append("\n")

    x, y = x0, y0
    sx = -1 if x0 > x1 else 1
    sy = -1 if y0 > y1 else 1
    points = []

    if dx > dy:
        step_info.append("Since dx > dy, calculate initial pk as 2*dy - dx")
        pk = 2*dy - dx
        step_info.append(f"pk = 2*{dy} - {dx}")
        step_info.append(f"pk = {pk}")
        two_dy = 2*dy
        two_dy_dx = 2*(dy - dx)
        k = 0
        while x != x1:
            x += sx
            if pk < 0:
                step_info.append(f"\nStep k={k}: Given pk ({pk}) < 0")
                step_info.append("Calculate new pk as pk + 2*dy and new x as x + sx")
                step_info.append(f"new pk = {pk} + 2*{dy}")
                step_info.append(f"new pk = {pk + two_dy}")
                step_info.append(f"new x = {x-sx} + {sx}")
                step_info.append(f"new x = {x}")
                pk += two_dy
            else:
                y += sy
                step_info.append(f"\nStep k={k}: Given pk ({pk}) >= 0")
                step_info.append("Calculate new pk as pk + 2*dy - 2*dx and new x as x + sx, new y as y + sy")
                step_info.append(f"new pk = {pk} + 2*{dy} - 2*{dx}")
                step_info.append(f"new pk = {pk + two_dy_dx}")
                step_info.append(f"new x = {x-sx} + {sx}, new y = {y-sy} + {sy}")
                step_info.append(f"new x = {x}, new y = {y}")
                pk += two_dy_dx
            points.append((x, y, pk))
            k += 1
    else:
        step_info.append("Since dx <= dy, calculate initial pk as 2*dx - dy")
        pk = 2*dx - dy
        step_info.append(f"pk = 2*{dx} - {dy}")
        step_info.append(f"pk = {pk}")
        two_dx = 2*dx
        two_dx_dy = 2*(dx - dy)
        k = 0
        while y != y1:
            y += sy
            if pk < 0:
                step_info.append(f"\nStep k={k}: Given pk ({pk}) < 0")
                step_info.append("Calculate new pk as pk + 2*dx and new y as y + sy")
                step_info.append(f"new pk = {pk} + 2*{dx}")
                step_info.append(f"new pk = {pk + two_dx}")
                step_info.append(f"new y = {y-sy} + {sy}")
                step_info.append(f"new y = {y}")
                pk += two_dx
            else:
                x += sx
                step_info.append(f"\nStep k={k}: Given pk ({pk}) >= 0")
                step_info.append("Calculate new pk as pk + 2*dx - 2*dy and new x as x + sx, new y as y + sy")
                step_info.append(f"new pk = {pk} + 2*{dx} - 2*{dy}")
                step_info.append(f"new pk = {pk + two_dx_dy}")
                step_info.append(f"new x = {x-sx} + {sx}, new y = {y-sy} + {sy}")
                step_info.append(f"new x = {x}, new y = {y}")
                pk += two_dx_dy
            points.append((x, y, pk))
            k += 1
    return step_info, points

x1 = 1
y1 = 2
x2 = 5
y2 = 5
step_info, points = bresenham(x1, y1, x2, y2)

print("Step by Step Calculation:")
for info in step_info:
    print(info)

print("\nTable of Calculated Points:")
print("k\tp_k\t(x_k+1, y_k+1)")
for k, point in enumerate(points):
    print(f"{k}\t{point[2]}\t({point[0]}, {point[1]})")
