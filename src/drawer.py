import json
import numpy as np
import matplotlib.pyplot as plt
from configs import config
import os

class Drawer:
    def __init__(self):
        self.max_x = 10
        self.min_x = -10
    
    def draw_line(self, obj):
        k = obj.get('k')
        b = obj.get('b')

        x = np.array([self.min_x, self.max_x])
        y = k*x + b
        plt.plot(x, y, color=obj.get('color'))

    def draw_points(self, obj):
        x = []
        y = []
        points = obj.get('points')
        for p in points:
            x.append(p.get('x'))
            y.append(p.get('y'))
        
        self.max_x, self.min_x = max(self.max_x, max(x)), min(self.min_x, min(x))
        
        plt.scatter(np.array(x), np.array(y), color=obj.get('color'))
    
config_path = os.path.join(config.SRC_CONFIGS_DIR, 'config.json')
with open(config_path, 'r') as f:
    config_data = json.loads(f.read())

drawer = Drawer()
for obj in config_data['objects']:
    obj_type = obj['type']
    if obj_type == 'points':
        drawer.draw_points(obj)
for obj in config_data['objects']:
    obj_type = obj['type']
    if obj_type == 'line':
        drawer.draw_line(obj)

plt.savefig(os.path.join(config.SRC_CONFIGS_DIR + "/../outputs/", 'output.png'))