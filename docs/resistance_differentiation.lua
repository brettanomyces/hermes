-- see resistance_differentiation.jpeg for working
max_t = 30
min_t = 0
print(math.sqrt(((rt(min_t) * math.pow(rt(max_t), 2)) - (rt(max_t) * math.pow(rt(min_t), 2))) / (rt(max_t) - rt(min_t))))
