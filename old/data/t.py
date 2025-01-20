import json
import matplotlib.pyplot as plt
import numpy as np


def polar_to_cart(r, t):
    t = t - 180
    x = np.cos(t) * r
    y = np.sin(t) * r

    return [x, y]


id_map = {
    "70:B8:F6:9B:F6:A0": 1,
    "94:E6:86:F7:86:38": 2,
    "C8:2E:18:FD:09:40": 3,
    "C8:2E:18:FD:09:F0": 4,
    "C8:2E:18:FD:0D:4C": 5,
    "A0:B7:65:52:60:18": 6,
}

# monitors
with open("site_nodes.json") as f:
    data = json.load(f)

monitors = []
lats = []
lons = []
for d in data:
    new_d = {}
    new_d["id"] = id_map[d["MacAddress"]]

    new_d["x"] = round(float(d["latitude"]) - 35, 5)
    new_d["y"] = round(float(d["longitude"]) + 98, 5)
    monitors.append(new_d)

    lats.append(new_d["x"])
    lons.append(new_d["y"])

with open("monitors.json", "w") as f:
    f.write(json.dumps(monitors, indent=2))

# plt.scatter(lats, lons)
# plt.show()

# wind data
with open("node_ppm_data.json") as f:
    data = json.load(f)

wind_data = []
for d in data:
    try:
        x, y = polar_to_cart(float(d["windMPH"]), float(d["windDIR"]))
        new_d = {}
        new_d["id"] = id_map[d["NodeId"]]
        new_d["x"] = x
        new_d["y"] = y
    except:
        continue

    wind_data.append(new_d)

with open("wind.json", "w") as f:
    f.write(json.dumps(wind_data, indent=2))
