#include <cstdint>
#include <vector>
#include <fstream>
#include <filesystem>
#include <cmath>

#include "google/protobuf/duration.pb.h"
#include "ortools/constraint_solver/routing.h"
#include "ortools/constraint_solver/routing_enums.pb.h"
#include "ortools/constraint_solver/routing_index_manager.h"
#include "ortools/constraint_solver/routing_parameters.h"
using namespace  std;

struct Point
{
    double x;
    double y;
};
double dist(Point T1, Point T2)
{
    double dist = sqrt((T1.x - T2.x) * (T1.x - T2.x) + (T1.y - T2.y) * (T1.y - T2.y))
    return dist;
}
void readInput(std::vector<std::vector<double> >& mx, const int& N, const std::vector<Point> points)
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (i == j)
                mx[i][j] = 0;
            else
                mx[i][j] = dist(points[i], points[j]);
}
namespace operations_research
{
    struct DataModel
    {
        std::vector<std::vector<double>> distance_matrix;
        std::vector<int64_t> demands;
        std::vector<int64_t> trans_caps;
        int64_t num_vehicles = 0;
        RoutingIndexManager::NodeIndex depot{ 0 };
    };

    void PrintSolution(const DataModel& data, const RoutingIndexManager& manager,
        const RoutingModel& routing, const Assignment& solution)
    {
        int64_t total_distance{ 0 };
        int64_t total_load{ 0 };
        for (int vehicle_id = 0; vehicle_id < data.num_vehicles; ++vehicle_id)
        {
            int64_t index = routing.Start(vehicle_id);
            int64_t route_distance{ 0 };
            int64_t route_load{ 0 };
            std::stringstream route;
            while (routing.IsEnd(index) == false)
            {
                int64_t node_index = manager.IndexToNode(index).value();
                route_load += data.demands[node_index];
                int64_t previous_index = index;
                index = solution.Value(routing.NextVar(index));
                route_distance += routing.GetArcCostForVehicle(previous_index, index,
                    int64_t{ vehicle_id });
            }
            total_distance += route_distance;
            total_load += route_load;
        }
        std::cout << total_distance << std::endl;     // LOG(INFO) before

    }

    void VrpCapacity(DataModel init_data)
    {
        DataModel data = init_data;
        RoutingIndexManager manager(data.distance_matrix.size(), data.num_vehicles,
            data.depot);
        RoutingModel routing(manager);
        const int transit_callback_index = routing.RegisterTransitCallback(
            [&data, &manager](int64_t from_index, int64_t to_index) -> int64_t
            {
                int from_node = manager.IndexToNode(from_index).value();
                int to_node = manager.IndexToNode(to_index).value();
                return data.distance_matrix[from_node][to_node];
            });
        routing.SetArcCostEvaluatorOfAllVehicles(transit_callback_index);
        const int demand_callback_index = routing.RegisterUnaryTransitCallback(
            [&data, &manager](int64_t from_index) -> int64_t
            {

                int from_node = manager.IndexToNode(from_index).value();
                return data.demands[from_node];
            });
        routing.AddDimensionWithVehicleCapacity(demand_callback_index, int64_t{ 0 },data.trans_caps,true,"Capacity");
        RoutingSearchParameters search_parameters = DefaultRoutingSearchParameters();
        search_parameters.set_first_solution_strategy(FirstSolutionStrategy::PATH_CHEAPEST_ARC);search_parameters.set_local_search_metaheuristic(LocalSearchMetaheuristic::GUIDED_LOCAL_SEARCH);
        search_parameters.mutable_time_limit()->set_seconds(1);
        const Assignment* solution = routing.SolveWithParameters(search_parameters);
        PrintSolution(data, manager, routing, *solution);
    }
}
int main(int argc, char** argv)
{
    std::string path = "C:/Users/Liza/Desktop/CVRP/data";
    auto it = filesystem::directory_iterator(path);
    std::vector<filesystem::path> array_path;
    copy_if(filesystem::begin(it), filesystem::end(it), std::back_inserter(array_path),
        [](const auto& entry)
        {
            return filesystem::is_regular_file(entry);
        });
    for (auto& p : array_path)
    {
        std::ifstream fin;
        fin.open(p.string());
        std::cout << p.string() << std::endl;
        int64_t N, n_trans_one, trans_caps_one;
        fin >> N >> n_trans_one >> trans_caps_one;
        std::vector<int64_t> buffer(n_trans_one, trans_caps_one);
        operations_research::DataModel data;
        data.trans_caps = buffer;
        data.num_vehicles = n_trans_one;
        std::vector<Point> points(N);
        std::vector<int64_t> ker(N);

        for (int i = 0; i < N; i++)
        {
            int64_t demand;
            Point p;
            fin >> demand >> p.x >> p.y;
            points[i] = p;
            ker[i] = demand;
        }
        std::vector<std::vector<double> > mx(N, std::vector<double>(N));
        readInput(mx, N, points);
        data.demands = ker;
        data.distance_matrix = mx;
        operations_research::VrpCapacity(data);
    }
    return EXIT_SUCCESS;
}
