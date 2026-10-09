# import math
# # Simulated ADAS sensor tracks
# tracks = [
#     {"id": 101, "x": 12.0, "y": 3.0},
#     {"id": 102, "x": 7.0, "y": -2.0},
#     {"id": 103, "x": 20.0, "y": 4.0}
# ]
# def calculate_distance(x, y):
#     return math.sqrt(x*x + y*y)
# print("ADAS Object Tracking System")
# print("---------------------------")
# for track in tracks:
#     distance = calculate_distance(track["x"], track["y"])
#     print(f"Track ID: {track['id']}")
#     print(f"Distance: {distance:.2f} meters")
#     if distance < 10:
#         print("Status: Nearby Object")
#     else:
#         print("Status: Safe Distance Threshold")
#     print("---------------------------")



import math
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Simulated moving objects
# x = forward position, y = lateral position
# vx and vy = velocity components in m/s
tracks = [
    {"id": 101, "x": 12.0, "y": 3.0, "vx": -1.0, "vy": 0.0},
    {"id": 102, "x": 7.0, "y": -2.0, "vx": 0.5, "vy": 0.0},
    {"id": 103, "x": 20.0, "y": 4.0, "vx": -2.0, "vy": -0.2}
]

dt = 0.5  # Simulation step in seconds

fig, ax = plt.subplots(figsize=(8, 7))

def update(frame):
    ax.clear()

    # Ego vehicle at origin
    ax.scatter(0, 0, c="blue", s=250,
               marker="s", label="Ego Vehicle")

    for track in tracks:
        track["x"] += track["vx"] * dt
        track["y"] += track["vy"] * dt

        distance = math.hypot(track["x"], track["y"])
        color = "red" if distance < 10 else "green"

        ax.scatter(track["y"], track["x"],
                   c=color, s=150)

        ax.annotate(
            f"ID {track['id']}\n{distance:.1f} m",
            (track["y"], track["x"]),
            xytext=(8, 8),
            textcoords="offset points"
        )

    ax.set_xlim(-15, 15)
    ax.set_ylim(-10, 30)
    ax.set_xlabel("Lateral Position (m)")
    ax.set_ylabel("Forward Position (m)")
    ax.set_title(f"ADAS Tracking - Frame {frame + 1}")
    ax.grid(True)
    ax.legend()

animation = FuncAnimation(
    fig,
    update,
    frames=30,
    interval=500,
    repeat=False,
    cache_frame_data=False
)

plt.show()
