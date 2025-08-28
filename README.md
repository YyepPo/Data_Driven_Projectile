# Data_Driven_Projectile
A lightweight, high-performance projectile system for Unreal Engine 5, using a data-driven approach with a central bullet manager and struct array. Supports lots of bullets with minimal CPU overhead, leveraging line traces and Niagara visuals

Features
  # Projectile
- Projectile system with realistic gravity-based bullet drop.
- Customizable projectile types via Data Assets for speed, Niagara visuals, and mass.
- Extensible to particles, AI crowds, inventories, and more.
- Managed by UTickableWorldSubsystem for global, actor-free logic.

  # Projectile Spawner (created in blueprints)
- Projectile spawner with selectable actor-based or data-driven spawning to compare FPS performance.
- Adjustable fire rate for projectile spawner
