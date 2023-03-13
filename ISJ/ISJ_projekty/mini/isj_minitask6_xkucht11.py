class Point(object):

    def __init__(self, x=0, y=0):
        self.x = x
        self.y = y

    def __sub__(self, other):
        x_ret = self.x - other.x
        y_ret = self.y - other.y
        return Point(x_ret, y_ret)

    def __str__(self):
        return f'Point({self.x}, {self.y})'


p0 = Point()
print(p0)
p1 = Point(3, 4)
print(p0 - p1)
p2 = Point(1, 2)
result = p1 - p2
print(result)
