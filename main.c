#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  const double PI = 3.141592653589793;
  const double CAPITAL_PHI = 45.5 * PI / 180; // local latitude

  // collector's polar angle
  const double theta_c = argc >= 2 ? atoi(argv[1]) * PI / 180 : PI / 9;
  // collector's azimuthal angle, clockwise from north
  const double phi_c = argc >= 3 ? atoi(argv[2]) * PI / 180 : PI;

  const int START_OF_SPRING = 79;
  const int START_OF_AUTUMN = 265;

  // Cartesian coordinates of the collector's normal vector
  const double x_c = sin(theta_c) * cos(phi_c);
  const double y_c = sin(theta_c) * sin(phi_c);
  const double z_c = cos(theta_c);

  double total_energy = 0;

  printf("Day     Energy\n");
  printf("================\n");

  for (int N = START_OF_SPRING; N < START_OF_AUTUMN; N++) {
    // declination of the Sun
    double delta = -23.44 * cos(2 * PI / 365 * (N + 10)) * PI / 180;

    const double INCREMENT = 0.01;
    double integral = 0.0;

    for (double hour = 4; hour <= 20; hour += INCREMENT) {
      double h = (hour - 12) * PI / 12;
      double cos_theta_s = sin(CAPITAL_PHI) * sin(delta) +
                           cos(CAPITAL_PHI) * cos(delta) * cos(h);

      // solar zenith angle
      double theta_s = acos(cos_theta_s);

      // phi_s = solar azimuth angle
      double sin_phi_s = -sin(h) * cos(delta) / sin(theta_s);
      double cos_phi_s = (sin(delta) * cos(CAPITAL_PHI) -
                          cos(h) * cos(delta) * sin(CAPITAL_PHI)) /
                         sin(theta_s);

      // Cartesian coordinates of the Sun on a unit sphere
      double x_s = sin(theta_s) * cos_phi_s;
      double y_s = sin(theta_s) * sin_phi_s;
      double z_s = cos_theta_s;

      // alpha = angle between the rays of the Sun and the collector's normal
      double cos_alpha = x_c * x_s + y_c * y_s + z_c * z_s;

      // only add if rays are hitting the collector
      if (cos_alpha > 0 && z_s > 0) {
        integral += cos_alpha * INCREMENT;
      }
    }

    total_energy += integral;
    if ((N - START_OF_SPRING) % 14 == 0) {
      printf("%3d %8.2f\n", N, integral);
    }
  }

  const double average_energy =
      total_energy / (START_OF_AUTUMN - START_OF_SPRING);
  printf("\nAverage: %.2f\n", average_energy);

  return 0;
}