
import csv
from collections import defaultdict
from pathlib import Path

import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from matplotlib.patches import Rectangle

CSV_FILE = Path(__file__).parent / "tracking_results.csv"

# Read C++ tracking results
frames = defaultdict(list)
history = defaultdict(list)

with CSV_FILE.open(newline="") as file:
    reader = csv.DictReader(file)

    for row in reader:
        frame = int(row["frame"])
        track = {
            "id": int(row["id"]),
            "x": float(row["x"]),
            "y": float(row["y"]),
            "vx": float(row["vx"]),
            "vy": float(row["vy"]),
            "missed": int(row["missed"])
        }

        frames[frame].append(track)
        history[track["id"]].append(
            (frame, track["y"], track["x"])
        )

frame_numbers = sorted(frames)

fig, ax = plt.subplots(figsize=(8, 9))


def update(frame):
    ax.clear()

    # Road background
    ax.set_facecolor("#202b3a")
    ax.axvline(0, color="white", linestyle="--", alpha=0.4)

    # Ego vehicle
    ego = Rectangle(
        (-0.9, -1.5), 1.8, 3,
        facecolor="#3b82f6",
        edgecolor="white"
    )
    ax.add_patch(ego)
    ax.text(1.1, 0, "EGO", color="white")

    for track in frames[frame]:
        x = track["x"]
        y = track["y"]
        track_id = track["id"]

        color = (
            "#ef4444" if track["missed"] > 0
            else "#22c55e"
        )

        # Tracked vehicle
        vehicle = Rectangle(
            (y - 0.8, x - 1.5),
            1.6, 3,
            facecolor=color,
            edgecolor="white"
        )
        ax.add_patch(vehicle)

        ax.text(
            y + 1, x,
            f"ID {track_id}\n"
            f"Speed X: {track['vx']:.1f} m/s",
            color="white",
            fontsize=9
        )

        # Motion trail
        trail = [
            (ty, tx)
            for f, ty, tx in history[track_id]
            if f <= frame
        ]

        if len(trail) > 1:
            ys, xs = zip(*trail)
            ax.plot(ys, xs, color=color, linewidth=2, alpha=0.7)

        # Velocity direction
        ax.arrow(
            y, x,
            track["vy"] * 2,
            track["vx"] * 2,
            head_width=0.3,
            color="yellow",
            length_includes_head=True
        )

    ax.set_xlim(-8, 8)
    ax.set_ylim(-4, 26)
    ax.set_aspect("equal", adjustable="box")
    ax.set_xlabel("Lateral Y (meters)")
    ax.set_ylabel("Longitudinal X (meters)")
    ax.set_title(
        f"ADAS Multi-Object Tracking | Frame {frame}"
    )
    ax.grid(alpha=0.15)


animation = FuncAnimation(
    fig,
    update,
    frames=frame_numbers,
    interval=1000,
    repeat=True,
    cache_frame_data=False
)

plt.show()
