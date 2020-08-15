#include "ez/AAHyperBox.h"
#include "ez/AAHyperCube.h"
#include "ez/Capsule.h"
#include "ez/Cylinder.h"
#include "ez/HyperSphere.h"
#include "ez/Line.h"
#include "ez/Macros.h"
#include "ez/MathForward.h"
#include "ez/MathIntersections.h"
#include "ez/Plane.h"
#include "ez/Ray.h"
#include "ez/Segment.h"

using namespace ez;

int main(int argc, const char** argv)
{
  Line2f line2;
  Ray2f ray2;
  Segment2f segment2;
  Circlef circle;
  AASquaref aasquare;
  AARectf aarect;
  Capsule2f capsule2;

  Line3f line3;
  Ray3f ray3;
  Segment3f segment3;
  Planef plane;
  Spheref sphere;
  AACubef aacube;
  AABoxf aabox;
  Capsule3f capsule3;
  Cylinderf cylinder;

  // Line2
  {
    // IntersectCheck
    PEEK(IntersectCheck(line2, line2));
    PEEK(IntersectCheck(line2, ray2));
    PEEK(IntersectCheck(line2, segment2));
    PEEK(IntersectCheck(line2, circle));
    PEEK(IntersectCheck(line2, aasquare));
    PEEK(IntersectCheck(line2, aarect));
    PEEK(IntersectCheck(line2, capsule2));

    // IntersectClosest
    PEEK(IntersectClosest(line2, line2));
    PEEK(IntersectClosest(line2, ray2));
    PEEK(IntersectClosest(line2, segment2));
    PEEK(IntersectClosest(line2, circle));
    PEEK(IntersectClosest(line2, aasquare));
    PEEK(IntersectClosest(line2, aarect));
    PEEK(IntersectClosest(line2, capsule2));

    // IntersectAll
    PEEK(IntersectAll(line2, line2));
    PEEK(IntersectAll(line2, ray2));
    PEEK(IntersectAll(line2, segment2));
    PEEK(IntersectAll(line2, circle));
    PEEK(IntersectAll(line2, aasquare));
    PEEK(IntersectAll(line2, aarect));
    PEEK(IntersectAll(line2, capsule2));

    // ClosestPoint
    PEEK(ClosestPoint(line2, line2));
    PEEK(ClosestPoint(line2, ray2));
    PEEK(ClosestPoint(line2, segment2));
    /*
    PEEK(ClosestPoint(line2, circle));
    PEEK(ClosestPoint(line2, aasquare));
    PEEK(ClosestPoint(line2, aarect));
    PEEK(ClosestPoint(line2, capsule2));
    */

    // Distance
    /*
    PEEK(SqDistance(line2, line2));
    PEEK(SqDistance(line2, ray2));
    PEEK(SqDistance(line2, segment2));
    PEEK(SqDistance(line2, circle));
    PEEK(SqDistance(line2, aasquare));
    PEEK(SqDistance(line2, aarect));
    PEEK(SqDistance(line2, capsule2));
    */
  }
}