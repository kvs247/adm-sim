import json

def write_data():
    with open("monitors.json") as f:
        data = json.load(f)
    print(data)

if __name__ == "__main__":
    write_data()