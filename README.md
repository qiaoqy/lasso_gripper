# Lasso Gripper: A String Shooting-Retracting Mechanism for Shape-Adaptive Grasping
## Abstract
Handling oversized, variable-shaped, or delicate objects in transportation, grasping tasks is extremely challenging, mainly due to the limitations of the gripper's shape and size. This paper proposes a novel gripper, Lasso Gripper. Inspired by traditional tools like the lasso and the uurga, Lasso Gripper captures objects by launching and retracting a string. Contrary to antipodal grippers, which concentrate force on a limited area, Lasso Gripper applies uniform pressure along the length of the string for a more gentle grasp. The gripper is controlled by four motors—two for launching the string inward and two for launching it outward. By adjusting motor speeds, the size of the string loop can be tuned to accommodate objects of varying sizes, eliminating the limitations imposed by the maximum gripper separation distance. To address the issue of string tangling during rapid retraction, a specialized mechanism was incorporated. Additionally, a dynamic model was developed to estimate the string's curve, providing a foundation for the kinematic analysis of the workspace. In grasping experiments, Lasso Gripper, mounted on a robotic arm, successfully captured and transported a range of objects, including bull and horse figures as well as delicate vegetables. The demonstration video is available here: [https://youtu.be/PV1J76mNP9Y](https://youtu.be/PV1J76mNP9Y).

## Hardware Design Specifications and Control Algorithms

The hardware design specifications and associated control algorithms required to replicate the experimental results have been deposited in this open-source repository.

Please change the network settings and IP address according to your own configuration in the file `full_driver/full_driver.ino`.

We selected the FOC drive boards from this open-source design for the retraction mechanism: [FOC Drive Boards](https://oshwhub.com/a1804889557/xin-chun-kuai-le-ning-meng-FOCka).

<img src="https://github.com/qiaoqy/lasso_gripper/blob/main/img/overall_img.png" alt="image" width="300">