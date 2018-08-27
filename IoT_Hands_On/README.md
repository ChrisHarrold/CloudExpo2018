# Particle_Projects
Particle Photon projects repo

New Repo for Particle Photon projects - will follow similar logic to other repos

Current Folders:

  - Hello World - contains simple .ino file for blinking LEDs with a Photon
  
  - SM_Prototype_Code - first prototype code for the soil moisture readings - good testing code here for breadboard projects
  
  - SoilMoisture - the current codebase for the Photon and Soil Moisture probes - assumes you have a PCB prototype model board     
  - also has subdirs due to the Particle ADAM offline IDE creating folders all on its own all the damn time:
    - Prototype - this is the primary .ino files I am working with
      - full_code.ino - INCOMPLETE! Do not use this one without checking the commit notes. Will be the final version someday
      - no_offline.ino - working code for a single unit without offline allowances made (in other words if your particle
      is not connected, you will NOT get any stored results from it if it is offline for long periods)
    
    - src - ADAM created this, it's just a duplicate of the SM_Prototype_Code folder for some reason...
