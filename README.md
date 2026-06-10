# Cosmic Wars 🚀

*An interactive, closed-loop electromechanical arcade game bridging physical robotics with educational gameplay.*

## 📖 1. Introduction & Educational Philosophy

**Cosmic Wars** is a fully automated, hardware-based educational game engineered specifically for children aged 2-10. The core mission of this project is to gamify learning by rewarding academic achievement with physical, interactive arcade action. 

By forcing players to engage in active recall to solve simple arithmetic questions, the system teaches fundamental math skills and introduces the basic concept of conditional logic (e.g., *If Answer = Correct, Then Release Ball*). A digital counter tracks successful target hits to foster a competitive and motivating learning environment.

## 🎮 2. The Gameplay Experience

The system pits two players against each other in a race to solve math challenges. 

1. **The Challenge:** An arithmetic equation is displayed on the central LCD screen. 
2. **The Input:** Players must quickly calculate the answer and input it using their designated arcade buttons.
3. **The Reward:** The first player to submit the correct answer is rewarded with system authorization. This unlocks their joystick and activates the dual-wheel shooting mechanism.
4. **The Action:** The authorized player aims their launcher and fires at physical targets. 
5. **The Loop:** The machine automatically collects, sorts, and recycles the fired balls back to the shooting station, creating a continuous, hands-free loop for the users.

## ⚙️ 3. Hardware Subsystem Deep Dives

The physical ball-handling process is a feat of integrated mechatronics, divided into three autonomous phases:

### Phase A: Automated Sorting
Once a ball strikes the target area, it drops onto a precisely angled, gravity-fed recovery track. 
* **Detection:** As the ball rolls down the incline, it passes a TCS3200 color sensor. The sensor rapidly identifies if the ball is white or green and sends this telemetry to the main controller.
* **Routing:** Based on the color data, the controller actuates a designated servo gate. This gate physically switches the track path, dropping the ball into its corresponding, color-matched queue.

### Phase B: Pneumatic Lifting
To elevate the sorted balls back to the shooting station, the system utilizes a vertical transport mechanism.
* **Actuation:** Once a ball is read by the color sensor, a delayed-release relay is triggered. This relay activates a high-pressure air blower.
* **Transportation:** The blower generates enough upward pneumatic force to propel the ball vertically through a clear acrylic tube. 
* **Aesthetics:** The clear tubing provides an airtight seal while allowing players to visually track the "lifting" phase of the game. 

### Phase C: The Shooting Mechanism
The final stage of the closed-loop system launches the queued balls back at the targets.
* **Aiming:** Authorized players control a panning platform that rotates left and right, driven by an MG996R servo motor.
* **Feeding:** Once aimed, a 5V solenoid linear actuator fires, pushing the queued ball forward into the launcher.
* **Propulsion:** The launcher consists of friction-enhanced rubber wheels driven by two high-speed DC motors. To achieve the necessary velocity, the standard 48:1 gearboxes were removed from 650 RPM motors, resulting in a staggering base motor speed of 31,200 RPM.

## 📊 4. Technical Specifications & Physics

Extensive actuator sizing, flow rate calculations, and trajectory simulations were conducted to ensure system reliability and accuracy.

* **System Dimensions:** 75 cm x 50 cm x 50 cm (Length x Width x Height).
* **Launch Velocity:** Running at a PWM duty cycle of 90/255, the actual RPM sits at ~11,011 RPM, achieving a consistent projectile launch velocity of 3.8 m/s.
* **Trajectory:** MATLAB simulations determined that a 30-degree launch angle utilizing a fixed ramp is optimal for hitting the designated targets accurately.
* **Materials:** The chassis and housings are constructed from laser-cut wood, with clear acrylic for the pneumatic lift, and custom 3D-printed passages for the ball routing. 

## 🛡️ 5. Mechanical & Electrical Safety

Given the target demographic, safety was a primary design constraint.
* **Containment:** All high voltage and high-current electrical components are strictly covered and isolated from user touch.
* **Mechanical Guards:** The high-powered air blower intake is fitted with a specialized mesh guard to completely prevent the accidental ingestion of fingers, hair, or loose debris.
* **Durability:** The system is engineered for a 2-year operational lifespan under heavy arcade use. 

## 👥 6. Authors & Contributors

Developed as a Mechatronics Engineering project by:
* **Mahmoud Elzeftawy** (24p0316)
* **Ahmed Hassan Abdel Aziz** (24P0331)
* **Kireyah Mouris Mosaad** (24P0273)
* **Elaria Ehab William** (25P0153)
* **Mina Maged** (24P0250)
* **Aya Allah Mohamed** (2200627)

## 📄 7. License

This is a dual-licensed open-source project:
* **Software:** All source code (Arduino C++ / Processing Java) is licensed under the [MIT License](LICENSE-MIT).
* **Hardware:** All CAD files, 3D models, and PCB schematics are licensed under [CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/).
