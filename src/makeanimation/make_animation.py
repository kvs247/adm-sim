from PIL import Image
from typing import Tuple, List
import io
import json
import matplotlib.pyplot as plt
import numpy as np
import numpy.typing as npt
import sys


def get_config(config_file_path: str) -> dict:
    with open(config_file_path) as f:
        return json.load(f)


def get_colormap(name: str, n_colors: int = 256) -> List[Tuple[int, int, int]]:
    cmap = plt.get_cmap(name)
    colors = cmap(np.linspace(0, 1, n_colors))

    colormap = []
    for color in colors:
        colormap.append((
            int(255 * color[0]),
            int(255 * color[1]),
            int(255 * color[2])
        ))
    return colormap


def generate_gif(data: npt.NDArray[np.float32], duration=75) -> io.BytesIO:
    animation_max = np.max(data)

    colormap = np.array(get_colormap('magma'), dtype=np.uint8)

    percent_blue = 5
    blue_entries = int(256 * (percent_blue / 100))
    colormap[:blue_entries] = colormap[0]

    buffer = io.BytesIO()
    images = []
    for frame in data:
        img = create_image(frame, colormap, animation_max)
        img = resize_image(img)
        images.append(img)

    images[0].save(
        buffer,
        format="GIF",
        save_all=True,
        append_images=images[1:],
        duration=duration,
        loop=0,
        optimize=False
    )

    buffer.seek(0)
    return buffer


def create_image(
    frame: npt.NDArray[np.float32],
    colormap: npt.NDArray[np.uint8],
    max_value: float,
) -> Image.Image:
    normalized_frame = np.clip(frame / max_value, 0, 1)
    indices = np.clip((normalized_frame * 255), 0, 255).astype(int)
    colored_frame = colormap[indices]

    opacity = np.full(frame.shape, 255, dtype=np.uint8)
    rgba_frame = np.dstack((colored_frame, opacity))

    rgba_frame = np.flipud(rgba_frame)

    return Image.fromarray(rgba_frame, mode="RGBA")


def resize_image(image: Image.Image) -> Image.Image:
    return image.resize((512, 512), Image.Resampling.LANCZOS)


def main():
    if len(sys.argv) != 2:
        print("Error: no imput to python make_animation.py")
        sys.exit(1)

    config = get_config("../config.json")

    input_data_path = sys.argv[1]

    dim = config["ANIMATION_DIMENSION"]
    data = np.fromfile(input_data_path, dtype=np.float32).reshape(-1, dim, dim)
    gif_buffer = generate_gif(data)
    with open("out/out.gif", "wb") as f:
        f.write(gif_buffer.getvalue())


if __name__ == "__main__":
    print("python....")
    main()
