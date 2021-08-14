#include "Utils.h"
#include "FileException.h"

std::random_device Utils::device;
std::mt19937 Utils::mt(device());
std::uniform_real_distribution<double> Utils::random(0.0, 1.0);

uint32_t Utils::getRandomUInt(uint32_t from, uint32_t to)
{
    std::uniform_int_distribution<uint32_t> randomInt(from, to);
    return randomInt(mt);
}

std::vector<std::vector<Edge>> Utils::parsePointsToEdges(const std::vector<Point>& points) noexcept
{
    std::vector<std::vector<Edge>> edges(
            CITIES_NUMBER,
            std::vector<Edge>(CITIES_NUMBER)
    );
    for (size_t i = 0; i < CITIES_NUMBER; ++i)
        for (size_t j = 0; j < CITIES_NUMBER; ++j)
            edges[i][j].setLength(distance(points[i], points[j]));

    return edges;
}

std::vector<Point> Utils::readPoints(std::ifstream& fin)
{
    std::vector<Point> points(CITIES_NUMBER);
    skipLines(fin, 2);
    for (size_t i = 0; i < CITIES_NUMBER; ++i)
    {
        fin >> points[i].x >> points[i].y;
        fin.ignore();
        FILE_THROW_IF_FAILED(fin, "Error during reading point number: " + std::to_string(i));
    }
    return points;
}

std::string Utils::timeToString(uint64_t microseconds) noexcept
{
    uint16_t micros = microseconds % 1000;
    microseconds /= 1000;
    uint16_t millis = microseconds % 1000;
    microseconds /= 1000;
    uint16_t seconds = microseconds % 60;
    microseconds /= 60;
    uint16_t minutes = microseconds % 60;

    std::string time;
    if (micros > 0)
    {
        time = std::to_string(micros) + "us " + time;
    }
    if (millis > 0)
    {
        time = std::to_string(millis) + "ms " + time;
    }
    if (seconds > 0)
    {
        time = std::to_string(seconds) + "s " + time;
    }
    if (minutes > 0)
    {
        time = std::to_string(minutes) + "m " + time;
    }

    return time;
}

std::string Utils::antsPathToString(const std::vector<size_t>& path) noexcept
{
    std::string result = "[";
    for (size_t i = 0; i < path.size(); i++)
    {
        if (i < path.size() - 1)
        {
            result += std::to_string(path[i]) + ",";
        } else
        {
            result += std::to_string(path[i]);
        }
    }
    result += "]";
    return result;
}

double Utils::distance(const Point& p1, const Point& p2) noexcept
{
    return std::sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

bool Utils::enoughFiles(uint32_t amount)
{
    std::fstream file;
    for (uint32_t i = 0; i < amount; i++)
    {
        file.open("in/" + std::to_string(i) + ".ant");
        if (!file.good())
            return false;
        file.close();
    }
    return true;
}

void Utils::skipLines(std::ifstream& fin, std::size_t lines)
{
    std::string s;
    for (; lines; --lines)
    {
        std::getline(fin, s);
        FILE_THROW_IF_FAILED(fin, "Error during skipping the lines");
    }
}

void Utils::setParameters(std::ifstream& fin)
{
    skipLines(fin, 1);
    fin >> ANTS_NUMBER;
    fin.ignore();
    FILE_THROW_IF_FAILED(fin, "Error during reading ANTS_NUMBER");
    skipLines(fin, 1);
    fin >> CITIES_NUMBER;
    fin.ignore();
    FILE_THROW_IF_FAILED(fin, "Error during reading CITIES_NUMBER");
    skipLines(fin, 1);
    fin >> ITERATION_NUMBER;
    fin.ignore();
    FILE_THROW_IF_FAILED(fin, "Error during reading ITERATION_NUMBER");
    skipLines(fin, 1);
    fin >> TAU0;
    fin.ignore();
    FILE_THROW_IF_FAILED(fin, "Error during reading TAU0");
    skipLines(fin, 1);
    fin >> ALPHA;
    fin.ignore();
    FILE_THROW_IF_FAILED(fin, "Error during reading ALPHA");
    skipLines(fin, 1);
    fin >> BETA;
    fin.ignore();
    FILE_THROW_IF_FAILED(fin, "Error during reading BETA");
    skipLines(fin, 1);
    fin >> Q0;
    fin.ignore();
    FILE_THROW_IF_FAILED(fin, "Error during reading Q0");
    skipLines(fin, 1);
    fin >> RHO;
    fin.ignore();
    FILE_THROW_IF_FAILED(fin, "Error during reading RHO");
    skipLines(fin, 1);
}

void Utils::getUserInput()
{
    char writeMode;

    std::cout << "Give the number of experiments:";
    std::cin >> EXPERIMENTS_NUMBER;

    if (!enoughFiles(EXPERIMENTS_NUMBER))
        throw std::invalid_argument("There are not enough *.ant files");

    std::cout << "Write only distances of paths in the output file (y/n):";
    std::cin >> writeMode;

    IS_NUMBERS_ONLY = (tolower(writeMode) == 'y');

    std::cout << "Do you want to add local actualization? (y/n):";
    std::cin >> writeMode;

    IS_LOCAL_UPDATE = (tolower(writeMode) == 'y');

    std::cout << "Do you want to add global actualization? (y/n):";
    std::cin >> writeMode;

    IS_GLOBAL_UPDATE = (tolower(writeMode) == 'y');
}

std::vector<Point> Utils::collectData(std::ifstream& fin)
{
    setParameters(fin);
    return readPoints(fin);
}