import math


def binomial_coeff(n, k):
    return round(math.factorial(n) / (math.factorial(k) * math.factorial(n - k)), 2)


def bezier_point(t, control_points):
    n = len(control_points) - 1
    x = 0
    y = 0

    step_by_step_x = []
    step_by_step_y = []

    for i in range(n + 1):
        bin_coeff = binomial_coeff(n, i)
        term_x = round(bin_coeff * ((1 - t) ** (n - i)) * (t ** i) * control_points[i][0], 2)
        term_y = round(bin_coeff * ((1 - t) ** (n - i)) * (t ** i) * control_points[i][1], 2)
        x += term_x
        y += term_y
        step_by_step_x.append(term_x)
        step_by_step_y.append(term_y)

    return round(x, 2), round(y, 2), step_by_step_x, step_by_step_y


def generate_formula(t, control_points, bin_coeffs, axis):
    return ' + '.join(
        [f'{bin_coeffs[i]} * (1-{t})^{len(control_points) - 1 - i} * {t}^{i} * {control_points[i][axis]}' for i in
         range(len(control_points))])


def print_bezier_steps(t, control_points):
    x, y, step_by_step_x, step_by_step_y = bezier_point(t, control_points)

    print(f"Titik untuk t = {t}")

    print("Formula Bezier untuk x dan y:")
    print("B(t) = Σ [n choose i] * (1 - t)^(n - i) * t^i * P_i")

    bin_coeffs = [binomial_coeff(len(control_points) - 1, i) for i in range(len(control_points))]

    print("Formula x:")
    print(f"x = {generate_formula(t, control_points, bin_coeffs, 0)}")
    x_terms = " + ".join([f"{term}" for term in step_by_step_x])
    print(f"x = {x_terms}")
    print(f"x = {x}")

    print("Formula y:")
    print(f"y = {generate_formula(t, control_points, bin_coeffs, 1)}")
    y_terms = " + ".join([f"{term}" for term in step_by_step_y])
    print(f"y = {y_terms}")
    print(f"y = {y}")

    print(f"Nilai titik pada kurva saat t = {t} adalah ({round(x)}, {round(y)})")


# control_points = [(1, 2), (7, 10), (15, 4)]
# t_increment = 0.02

# UK

control_points = [(0, 1), (1, 2), (2, 2), (3, 1)]
t_increment = 0.01

n_points = int(1 / t_increment)
print(f"n_points = 1 / {t_increment} = {n_points}")

print(f"Dengan kenaikan sebanyak {t_increment} maka jumlah titik yang diperlukan adalah {n_points}")

t = 0.0
print("Kasus 1 (3 titik kontrol):")
print_bezier_steps(t, control_points)
