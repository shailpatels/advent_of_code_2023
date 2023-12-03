
import re
import sys

sum = 0
p_set = 0
for line in sys.stdin:
    if len(line) <= 1:
        break 

    id,rest = line.strip().split(":")
    id = int(re.search(r'\d+',id).group())

    games = rest.split(";")
    is_good = True

    req_r = 0
    req_g = 0
    req_b = 0

    for game in games:
        parts = game.split(",")

        num_red = 0 
        num_green =0 
        num_blue = 0

        for part in parts:
            part = part.strip()
            if "red" in part:
                num_red = int(re.search(r'\d+', part).group())
            if "green" in part:
                num_green = int(re.search(r'\d+', part).group())
            if "blue" in part:
                num_blue = int(re.search(r'\d+', part).group())

            
        max_r = 12
        max_g = 13
        max_b = 14
        print("r ", num_red, " g ", num_green, "b ", num_blue)
        if num_red <= max_r and num_green <= max_g and num_blue <= max_b:
            pass
        else:
            is_good = False

        req_r = max(req_r, num_red)
        req_g = max(req_g, num_green)
        req_b = max(req_b, num_blue)

    print("game ", id, ": r ", req_r, " b ", req_b, " g ", req_g) 
    p_set += (req_r * req_g * req_b)
    if is_good:
        print(id , "is good")
        sum += id

print()
print(sum)
print(p_set)
