from PIL import Image
import io
import numpy as np
import numpy.typing as npt
from typing import Tuple, List


def jet() -> List[Tuple[int, int, int]]:
    """Generate a jet colormap (blue -> cyan -> yellow -> red)"""
    n = 256
    colormap = []
    for i in range(n):
        # Normalize i to [0,1]
        x = i / (n-1)
        r = np.clip(np.minimum(4*x - 1.5, -4*x + 4.5), 0, 1)
        g = np.clip(np.minimum(4*x - 0.5, -4*x + 3.5), 0, 1)
        b = np.clip(np.minimum(4*x + 0.5, -4*x + 2.5), 0, 1)

        # Convert to 0-255 range
        colormap.append((
            int(255 * r),
            int(255 * g),
            int(255 * b)
        ))
    return colormap


def generate_gif(data: npt.NDArray[np.float32], duration=75) -> io.BytesIO:
    animation_max = np.max(data)
    print("max: ", animation_max)

    # Create a modified jet colormap that starts with solid blue
    colormap = np.array(jet(), dtype=np.uint8)
    # Make the first ~25 entries solid blue for the bottom 10%
    percent_blue = 5
    blue_entries = int(256 * (percent_blue / 100))
    colormap[:blue_entries] = colormap[0]  # Set to the darkest blue

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
    # Normalize the frame to [0, 1]
    normalized_frame = np.clip(frame / max_value, 0, 1)

    # Create the color indices
    indices = np.clip((normalized_frame * 255), 0, 255).astype(int)

    # Apply the colormap
    colored_frame = colormap[indices]

    # Create fully opaque image
    opacity = np.full(frame.shape, 255, dtype=np.uint8)
    rgba_frame = np.dstack((colored_frame, opacity))

    # flip image vertically
    rgba_frame = np.flipud(rgba_frame)

    return Image.fromarray(rgba_frame, mode="RGBA")


def resize_image(image: Image.Image) -> Image.Image:
    return image.resize((512, 512), Image.Resampling.LANCZOS)


if __name__ == "__main__":
    data = np.fromfile("distributionData",
                       dtype=np.float32).reshape(-1, 200, 200)
    print(data.shape)
    gif_buffer = generate_gif(data)
    with open("out.gif", "wb") as f:
        f.write(gif_buffer.getvalue())
