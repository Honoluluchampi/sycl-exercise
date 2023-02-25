// compile command
// clang++ -fsycl -fsycl-targets=nvptx64-nvidia-cuda simple-sycl-app.cpp -L/usr/lib/gcc/x86_64-linux-gnu/11 -o simple-sycl-app -I /usr/include/c++/11 -I /usr/include/x86_64-linux-gnu/c++/11

#include <vector>

struct field
{
  int nx;
  int ny;
  double dx;
  double dy;
  std::vector<double> data;
};

void evole(field& curr, field& prev, double a, double dt)
{
  auto curr_data = curr->data.data();
  auto prev_data = prev->data.data();
  int nx = curr->nx;
  int ny = curr->ny;

  double dx2 = prev->dx * prev->dx;
  double dy2 = prev->dy * prev->dy;
  for (int i = 1; i < nx + 1; i++) {
    for (int j = 1; j < ny + 1; j++) {
      int ind = i * (ny + 2) + j;
      int ip = (i + 1) * (ny + 2) + j;
      int im = (i - 1) * (ny + 2) + j;
      int jp = i * (ny + 2) + j + 1;
      int jm = i * (ny + 2) + j - 1;
      curr_data[ind] = prev_data[ind] + a * dt *
      ((prev_data[ip] - 2.0 * prev_data[ind] + prev_data[im]) / dx2 + 
       (prev_data[jp] - 2.0 * prev_data[ind] + prev_data[jm]) / dy2);
    }
  }
}

void initialize(int argc, char* argv[], field& curr, field& prev, int& nsteps)
{

}

int main(int argc, char* argv[]) 
{
  int nsteps;
  field current, previous;
  initialize(argc, argv, &current, &previous, &nsteps);

  double a = 0.5;

  double dx2 = current.dx * current.dx;
  double dy2 = current.dy * current.dy;

  // largest stable time step
  double dt = dx2 * dy2 / (2.0 * a * (dx2 + dy2));
  auto start = wall_clock_t::now();

  for (int iter = 1; iter <= nsteps; iter++) {
    evole(current, previous, a, dt);
    if (iter % image_interval == 0) {

    }
  }
}