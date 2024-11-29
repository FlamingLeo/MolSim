#include "io/input/XMLReader.h"
#include "utils/PathUtils.h"
#include <gtest/gtest.h>
#define READ_XML(_x)                                                                                                   \
    do {                                                                                                               \
        XMLReader xml{targetPath + _x};                                                                                \
        xml.readXML(args, pc);                                                                                         \
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
    Arguments args;
    ParticleContainer pc;
    READ_XML("/testXMLValid_Complete.xml");

    // check arguments
    constexpr std::array domainSize{100.0, 100.0, 0.0};
    EXPECT_DOUBLE_EQ(args.startTime, 0.0);
    EXPECT_DOUBLE_EQ(args.endTime, 10.0);
    EXPECT_DOUBLE_EQ(args.delta_t, 0.01);
    EXPECT_DOUBLE_EQ(args.epsilon, 1.0);
    EXPECT_DOUBLE_EQ(args.sigma, 1.0);
    EXPECT_EQ(args.itFreq, 100);
    EXPECT_EQ(args.basename, "MD_vtk");
    EXPECT_EQ(args.type, WriterType::VTK);
    EXPECT_EQ(args.domainSize, domainSize);
    EXPECT_DOUBLE_EQ(args.cutoffRadius, 5.0);
    EXPECT_EQ(args.sim, SimulationType::LJ);

    // check objects
    // ASSERT_EQ(pc.size(), -1); // TODO: why 0,0,0?
    // std::cout << pc.toString() << "\n";

    /*
    { x: [3, 3, 0], v: [-0.0279367, -0.00286395, 0], f: [0, 0, 0], old_f: [0, 0, 0], m: 2, type: 0 },
    { x: [4, 3, 0], v: [-0.0969398, 0.1164, 0], f: [0, 0, 0], old_f: [0, 0, 0], m: 2, type: 0 },
    { x: [3, 4, 0], v: [-0.0161943, -0.0170185, 0], f: [0, 0, 0], old_f: [0, 0, 0], m: 2, type: 0 },
    { x: [4, 4, 0], v: [-0.0644259, -0.035936, 0], f: [0, 0, 0], old_f: [0, 0, 0], m: 2, type: 0 },

    { x: [5, 5, 0], v: [0.1, 0.2, 0], f: [0, 0, 0], old_f: [0, 0, 0], m: 0.5, type: 1 },
    { x: [6, 6, 0], v: [0.1, 0.2, 0], f: [0, 0, 0], old_f: [0, 0, 0], m: 0.5, type: 1 },

    { x: [0, 0, 0], v: [-0.0809618, -0.198221, 0], f: [0, 0, 0], old_f: [0, 0, 0], m: 1, type: 0 },
    { x: [10.5, 10, 0], v: [-0.0601211, -0.016068, 0], f: [0, 0, 0], old_f: [0, 0, 0], m: 1, type: 0 },
    { x: [10.433, 10.25, 0], v: [-0.140382, -0.324431, 0], f: [0, 0, 0], old_f: [0, 0, 0], m: 1, type: 0 },
    { x: [10.25, 10.433, 0], v: [-0.024801, -0.140229, 0], f: [0, 0, 0], old_f: [0, 0, 0], m: 1, type: 0 },
    { x: [10, 10.5, 0], v: [-0.0865893, -0.438648, 0], f: [0, 0, 0], old_f: [0, 0, 0], m: 1, type: 0 },
    { x: [9.75, 10.433, 0], v: [-0.128889, -0.275648, 0], f: [0, 0, 0], old_f: [0, 0, 0], m: 1, type: 0 },
    { x: [9.56699, 10.25, 0], v: [-0.0620944, -0.221427, 0], f: [0, 0, 0], old_f: [0, 0, 0], m: 1, type: 0 }
    */
}

// Test loading a valid, partial XML file with no arguments and only one object into a particle container.
TEST_F(XMLReaderTests, OpenFileValidPartial) {
    Arguments args;
    ParticleContainer pc;
    READ_XML("/testXMLValid_NoOptionals.xml");

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

    EXPECT_DEATH({ READ_XML("/testXMLInvalid_WrongTypes.xml"); }, "");
    EXPECT_DEATH({ READ_XML("/testXMLInvalid_GarbageTags.xml"); }, "");
    EXPECT_DEATH({ READ_XML("/testXMLInvalid_MissingType.xml"); }, "");
    EXPECT_DEATH({ READ_XML("/testXMLInvalid_MissingObjectsTag.xml"); }, "");
    EXPECT_DEATH({ READ_XML("/testXMLInvalid_MissingObjectsContents.xml"); }, "");
}