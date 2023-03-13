"""
author:
    Samuel Kuchta (xkucht11@stud.fit.vutbr.cz)
file:
    profiling.py
brief:
    Measures speed of MathLib implementation.
    Reads numbers from input, computes standard deviation, measures computation time, and function calls.
"""
from mat_lib import MathLib as Ml
import sys
import cProfile

try:
    with open(0) as f:  # read numbers from stdin.
        nums_read = [x for x in f]  # list of numbers in the nums variable.
except Exception as e:
    print(e)
    sys.exit(1)

nums_str = ""
for num in nums_read:
    nums_str = f"{nums_str} {num}"

nums = []
nums_list_of_strings = nums_str.split()
for string in nums_list_of_strings:
    nums.append(float(string))

cnt = len(nums)  # N

res_final = "Undefined Result: At least 2 numbers must be entered"
avg = 0
if cnt > 1:
    sum_n, sum_sq_n = 0, 0
    profiler = cProfile.Profile()
    profiler.enable()

    for i in range(0, cnt):
        sum_n = Ml.add(sum_n, nums[i])  # ∑xi
        sq_n = Ml.pow(nums[i], 2)  # xi^2
        sum_sq_n = Ml.add(sum_sq_n, sq_n)  # ∑xi^2

    mean = Ml.div(sum_n, cnt)  # x̄ = ∑xi / N
    sq_mean = Ml.pow(mean, 2)  # x̄^2
    sum_sq_mean = Ml.mul(cnt, sq_mean)  # N * x̄^2
    res_1 = Ml.sub(sum_sq_n, sum_sq_mean)  # ∑xi^2 - N * x̄^2
    cnt_dec = Ml.sub(cnt, 1)  # N-1
    res_2 = Ml.div(res_1, cnt_dec)  # ((∑xi^2 - N * x̄^2) / N-1)
    res_final = Ml.root(res_2)  # √((∑xi^2 - N * x̄^2) / N-1)

    profiler.disable()
    profiler.dump_stats("profiling.prof")

print(res_final)
