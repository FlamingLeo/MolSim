#include "io/input/XMLReader.h"
#include "utils/PathUtils.h"
#include <gtest/gtest.h>
#define READ_XML(_x)                                                                                                   \
    do {                                                                                                               \
        XMLReader xml{targetPath + _x};                                                                                \
        xml.readXML(args, pc, t);                                                                                      \
    } while (0)

class XMLReaderTests : public ::testing::Test {
  protected:
    std::string targetPath;

    void SetUp() override {
        std::string failReason = "";
        if (!PathUtils::setupFileTests(targetPath, failReason))
            GTEST_SKIP() << failReason;
    }
};

// Test loading a valid, complete XML file with arguments and multiple different objects into a particle container.
TEST_F(XMLReaderTests, OpenFileValidComplete) {
    constexpr std::array domainSize{100.0, 100.0, 0.0};
    constexpr std::array<std::array<double, 3>, 11> x = {{{3, 3, 0},
                                                          {4, 3, 0},
                                                          {3, 4, 0},
                                                          {4, 4, 0},
                                                          {5, 5, 0},
                                                          {6, 6, 0},
                                                          {10, 9, 0},
                                                          {9, 10, 0},
                                                          {10, 10, 0},
                                                          {11, 10, 0},
                                                          {10, 11, 0}}};
    constexpr std::array<double, 11> m = {2, 2, 2, 2, 0.5, 0.5, 1, 1, 1, 1, 1};
    constexpr std::array<std::array<double, 3>, 11> f = {{{0., 0., 0.},
                                                          {0., 0., 0.},
                                                          {0., 0., 0.},
                                                          {0., 0., 0.},
                                                          {0., 0., 0.},
                                                          {1., 2., 3.},
                                                          {0., 0., 0.},
                                                          {0., 0., 0.},
                                                          {0., 0., 0.},
                                                          {0., 0., 0.},
                                                          {0., 0., 0.}}};
    constexpr std::array<std::array<double, 3>, 11> oldF = {{{0., 0., 0.},
                                                             {0., 0., 0.},
                                                             {0., 0., 0.},
                                                             {0., 0., 0.},
                                                             {0., 0., 0.},
                                                             {4., 5., 6.},
                                                             {0., 0., 0.},
                                                             {0., 0., 0.},
                                                             {0., 0., 0.},
                                                             {0., 0., 0.},
                                                             {0., 0., 0.}}};

    Arguments args;
    ParticleContainer pc;
    Thermostat t{pc};
    READ_XML("/testXMLValid_Complete.xml");

    // check arguments
    EXPECT_DOUBLE_EQ(args.startTime, 0.0);
    EXPECT_DOUBLE_EQ(args.endTime, 10.0);
    EXPECT_DOUBLE_EQ(args.delta_t, 0.01);
    EXPECT_EQ(args.itFreq, 100);
    EXPECT_EQ(args.basename, "MD_vtk");
    EXPECT_EQ(args.type, WriterType::VTK);
    EXPECT_EQ(args.domainSize, domainSize);
    EXPECT_DOUBLE_EQ(args.cutoffRadius, 5.0);
    EXPECT_EQ(args.sim, SimulationType::LJ);

    // check thermostat
    EXPECT_DOUBLE_EQ(t.getInitTemp(), 40.0);
    EXPECT_EQ(t.getTimestep(), 1000);
    EXPECT_DOUBLE_EQ(t.getTargetTemp(), 60.0);
    EXPECT_DOUBLE_EQ(t.getDeltaT(), 0.5);

    // check objects
    ASSERT_EQ(pc.size(), 11);
    for (size_t i = 0; i < pc.size(); ++i) {
        EXPECT_EQ(pc[i].getX(), x[i]);
        EXPECT_EQ(pc[i].getM(), m[i]);
        EXPECT_EQ(pc[i].getF(), f[i]);
        EXPECT_EQ(pc[i].getOldF(), oldF[i]);
    }
}

// Test loading a valid, partial XML file with no arguments and only one object into a particle container.
TEST_F(XMLReaderTests, OpenFileValidPartial) {
    Arguments args;
    ParticleContainer pc;
    Thermostat t{pc};
    READ_XML("/testXMLValid_NoOptionals.xml");

    // check thermostat
    EXPECT_DOUBLE_EQ(t.getInitTemp(), 40.0);
    EXPECT_EQ(t.getTimestep(), 2147483647);

    // check inserted object
    ASSERT_EQ(pc.size(), 1);

    Particle &inserted = pc[0];

    constexpr std::array pos{5.0, 5.0, 0.0};
    constexpr std::array vel{0.1, 0.2, 0.0};
    EXPECT_EQ(inserted.getX(), pos);
    EXPECT_EQ(inserted.getV(), vel);
    EXPECT_DOUBLE_EQ(inserted.getM(), 0.5);
    EXPECT_EQ(inserted.getType(), 1);
}

// Test loading various invalid XML files. The program should exit upon trying to parse them.
TEST_F(XMLReaderTests, OpenFilesInvalid) {
    Arguments args;
    ParticleContainer pc;
    Thermostat t{pc};

    GTEST_FLAG_SET(death_test_style, "threadsafe");
    EXPECT_DEATH({ READ_XML("/testXMLInvalid_IncorrectStructure.xml"); }, "");
    EXPECT_DEATH({ READ_XML("/testXMLInvalid_NotXML.xml"); }, "");
    EXPECT_DEATH({ READ_XML("/testXMLInvalid_WrongTypes.xml"); }, "");
    EXPECT_DEATH({ READ_XML("/testXMLInvalid_GarbageTags.xml"); }, "");
    EXPECT_DEATH({ READ_XML("/testXMLInvalid_MissingType.xml"); }, "");
    EXPECT_DEATH({ READ_XML("/testXMLInvalid_MissingObjectsTag.xml"); }, "");
    EXPECT_DEATH({ READ_XML("/testXMLInvalid_MissingObjectsContents.xml"); }, "");
}