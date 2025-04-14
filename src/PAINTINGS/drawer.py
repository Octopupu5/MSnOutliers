import json
import numpy as np
import matplotlib.pyplot as plt
import sys
import os

class Drawer:
    def __init__(self, min_x=-10, max_x=10):
        self.min_x = min_x
        self.max_x = max_x

    def draw_line(self, obj):
        orientation = obj.get('orientation', None)
        if orientation == 'horizontal':
            x_values = np.array([obj.get('min_x', self.min_x), obj.get('max_x', self.max_x)])
            y_values = np.array([obj.get('y'), obj.get('y')])
            plt.plot(x_values, y_values, color=obj.get('color', 'black'), linewidth=obj.get('thickness', 1.0), alpha=obj.get('transparency', 1.0))
        elif orientation == 'vertical':
            y_values = np.array([obj.get('min_y', -10), obj.get('max_y', 10)])
            x_values = np.array([obj.get('x'), obj.get('x')])
            plt.plot(x_values, y_values, color=obj.get('color', 'black'), linewidth=obj.get('thickness', 1.0), alpha=obj.get('transparency', 1.0))
        else:
            k = obj.get('k')
            b = obj.get('b')
            x_values = np.array([self.min_x, self.max_x])
            y_values = k * x_values + b
            plt.plot(x_values, y_values, color=obj.get('color', 'green'), linewidth=obj.get('thickness', 1.0), alpha=obj.get('transparency', 1.0))

    def draw_points(self, obj):
        points = obj.get('points', [])
        x = [p.get('x') for p in points]
        y = [p.get('y') for p in points]
        plt.scatter(x, y, color=obj.get('color', 'red'), s=obj.get('pointSize', 5.0), alpha=obj.get('transparency', 1.0))
        if x: self.max_x, self.min_x = max(self.max_x, max(x)), min(self.min_x, min(x))

    def draw_function(self, obj):
        points = obj.get('points', [])
        if points:
            points_sorted = sorted(points, key=lambda p: p.get('x'))
            x = [p.get('x') for p in points_sorted]
            y = [p.get('y') for p in points_sorted]
            plt.scatter(x, y, color=obj.get('color', 'blue'), s=obj.get('pointSize', 5.0), alpha=obj.get('transparency', 1.0))

def main(config_path):
    with open(config_path, 'r') as f:
        config_data = json.load(f)
    
    drawer = Drawer()
    
    plt.figure()
    plt.title(config_data.get('title', ''))
    plt.xlabel(config_data.get('xLabel', ''))
    plt.ylabel(config_data.get('yLabel', ''))
    
    for obj in config_data.get('objects', []):
        if obj.get('type') == 'points':
            drawer.draw_points(obj)
    
    for obj in config_data.get('objects', []):
        obj_type = obj.get('type')
        if obj_type == 'line':
            drawer.draw_line(obj)
        elif obj_type == 'function':
            drawer.draw_function(obj)
    
    outputPath = config_data.get('outputPath', 'output.png')
    plt.savefig(outputPath)
    print('graph has been saved to', outputPath)

if __name__ == '__main__':
    config_path = sys.argv[1]
    main(config_path)