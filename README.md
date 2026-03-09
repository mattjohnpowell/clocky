# Clocky — A Kid-Friendly Sleep Training Clock

Clocky is a sleep training clock designed to help young children understand when it is time to wake up and when it is time to go to bed. It runs on a Guition JC8048W550C_I ESP32-S3 5-inch IPS touchscreen display and uses friendly Spider-Man characters and color-changing backgrounds to communicate the time of day in a way that is immediately understandable to children who cannot yet read a clock.

## What the app does

### Clock and date display
The main screen shows the current time in large, easy-to-read digits alongside the full date. The time is kept accurate automatically by connecting to the internet and synchronizing with an NTP time server.

### Day and night backgrounds
The background color of the screen changes to reflect whether it is currently day or night according to the child's personalized schedule:
- **Daytime** — a warm orange-to-yellow gradient signals that it is okay to be awake.
- **Night-time** — a calm light-blue-to-dark-blue gradient signals that it is time to sleep.

### Spider-Man bedtime animations
As bedtime approaches, a friendly Spider-Man character fades onto the screen to give the child advance notice that sleep is coming soon. Once bedtime has arrived, a sleeping Spider-Man image is shown so the child knows it is time to close their eyes.

### Customizable wake-up and bedtime
Parents can set personalized wake-up and bedtime values directly on the device using on-screen **+** and **−** buttons. Times adjust in 30-minute steps and are shown clearly on the main screen at all times.

### Parental controls
A PIN-protected settings area prevents children from changing the schedule themselves. After a short period of inactivity the settings lock automatically. Repeated wrong PIN entries trigger a temporary lockout to discourage guessing.

## Who it is for

Clocky is aimed at parents of toddlers and young children who need a simple, visual cue to know whether they should get out of bed or stay in their room. The large screen, bold colors, and familiar animated character make it easy for children to understand without any adult explanation.