#include "eigen_types.h"

using namespace pybind11::literals;

namespace g2o {

void declareEigenTypes(py::module& m) {
  py::class_<Eigen::Quaterniond>(m, "Quaternion")
      //.def(py::init<>())
      .def(py::init([]() { return Eigen::Quaterniond::Identity(); }))

      .def(py::init<const Eigen::Quaterniond&>())
      .def(py::init<const Eigen::AngleAxisd&>())
      .def(py::init<const Eigen::Matrix<double, 3, 3>&>())
      //.def(py::init<const Eigen::Matrix<double, 4, 1>&>())
      .def(py::init<const double&, const double&, const double&, const double&>(), "w"_a, "x"_a,
           "y"_a, "z"_a)

      .def(py::init([](const Eigen::Matrix<double, 4, 1>& m) {
        // return std::unique_ptr<Eigen::Quaterniond>(new Eigen::Quaterniond(m(3,0), m(0,0), m(1,0),
        // m(2,0)));
        return std::unique_ptr<Eigen::Quaterniond>(
            new Eigen::Quaterniond(m(0, 0), m(1, 0), m(2, 0), m(3, 0)));
      }))

      .def_static("from_two_vectors",
                  [](Eigen::Matrix<double, 3, 1>& a, Eigen::Matrix<double, 3, 1>& b) {
                    return Eigen::Quaterniond::FromTwoVectors(a, b);
                  })

      .def("x", (const double& (Eigen::Quaterniond::*)() const) & Eigen::Quaterniond::x)
      .def("y", (const double& (Eigen::Quaterniond::*)() const) & Eigen::Quaterniond::y)
      .def("z", (const double& (Eigen::Quaterniond::*)() const) & Eigen::Quaterniond::z)
      .def("w", (const double& (Eigen::Quaterniond::*)() const) & Eigen::Quaterniond::w)

      .def("vec", (const Eigen::VectorBlock<const Eigen::Quaterniond::Coefficients, 3> (
                      Eigen::Quaterniond::*)() const) &
                      Eigen::Quaterniond::vec)

      .def_static("identity", &Eigen::Quaterniond::Identity)
      .def("set_identity", [](Eigen::Quaterniond& q) { q.setIdentity(); })

      .def("rotation_matrix", &Eigen::Quaterniond::toRotationMatrix)
      .def("matrix", &Eigen::Quaterniond::toRotationMatrix)
      .def_property_readonly("R", &Eigen::Quaterniond::toRotationMatrix)

      .def("squared_norm", &Eigen::Quaterniond::squaredNorm)
      .def("norm", &Eigen::Quaterniond::norm)
      .def("normalize", &Eigen::Quaterniond::normalize)
      .def("normalized", &Eigen::Quaterniond::normalized)
      .def("dot", [](Eigen::Quaterniond& q1, Eigen::Quaterniond& q2) { return q1.dot(q2); })
      .def("angular_distance",
           [](Eigen::Quaterniond& q1, Eigen::Quaterniond& q2) { return q1.angularDistance(q2); })
      .def(py::self * py::self)
      .def(py::self *= py::self)
      .def("inverse", &Eigen::Quaterniond::inverse)
      .def("conjugate", &Eigen::Quaterniond::conjugate)
      .def("coeffs", (Eigen::Quaterniond::Coefficients & (Eigen::Quaterniond::*)()) &
                         Eigen::Quaterniond::coeffs)  // x, y, z, w

      .def("__mul__",
           [](Eigen::Quaterniond& q, Eigen::Matrix<double, 3, 1>& t) {
             // Eigen::Matrix<double,3,1> result = q * t;
             Eigen::Matrix<double, 3, 1> result = q._transformVector(t);
             return result;
           })
      //.def("__mul__", [](Eigen::Quaterniond& q, Eigen::Matrix<double,3,Eigen::Dynamic>& t) {
      //        return q * t;
      //    })

      ;

  py::class_<Eigen::Rotation2Dd>(m, "Rotation2d")
      //.def(py::init<>())
      .def(py::init([]() { return Eigen::Rotation2Dd::Identity(); }))
      .def(py::init<Eigen::Rotation2Dd&>())
      .def(py::init<const double&>())
      .def(py::init<const Eigen::Matrix<double, 2, 2>&>())

      .def("angle", (double& (Eigen::Rotation2Dd::*)()) & Eigen::Rotation2Dd::angle)
      .def("smallest_positive_angle", &Eigen::Rotation2Dd::smallestPositiveAngle)
      .def("smallest_angle", &Eigen::Rotation2Dd::smallestAngle)
      .def("inverse", &Eigen::Rotation2Dd::inverse)
      .def(py::self * py::self)
      .def(py::self *= py::self)
      .def(py::self * Eigen::Matrix<double, 2, 1>())
      //.def("from_rotation_matrix", (Eigen::Rotation2Dd& (Eigen::Rotation2Dd::*) (const
      //Eigen::Matrix<double, 2, 2>&)) 		&Eigen::Rotation2Dd::fromRotationMatrix)
      .def("from_rotation_matrix",
           [](Eigen::Rotation2Dd& r, const Eigen::Matrix<double, 2, 2>& R) {
             r.fromRotationMatrix(R);
           })
      .def("to_rotation_matrix", &Eigen::Rotation2Dd::toRotationMatrix)
      .def("rotation_matrix", &Eigen::Rotation2Dd::toRotationMatrix)
      .def("matrix", &Eigen::Rotation2Dd::toRotationMatrix)
      .def_property_readonly("R", &Eigen::Rotation2Dd::toRotationMatrix)
      .def("slerp", &Eigen::Rotation2Dd::slerp)
      .def_static("ientity", &Eigen::Rotation2Dd::Identity);

  py::class_<Eigen::AngleAxisd>(m, "AngleAxis")
      .def(py::init([]() { return Eigen::AngleAxisd::Identity(); }))
      .def(py::init<const double&, const Eigen::Matrix<double, 3, 1>&>())
      .def(py::init<const Eigen::AngleAxisd&>())
      .def(py::init<const Eigen::Quaterniond&>())
      .def(py::init<const Eigen::Matrix<double, 3, 3>&>())
      .def("angle", (double& (Eigen::AngleAxisd::*)()) & Eigen::AngleAxisd::angle)
      .def("axis",
           (Eigen::Matrix<double, 3, 1> & (Eigen::AngleAxisd::*)()) & Eigen::AngleAxisd::axis)
      .def(py::self * py::self)
      .def(py::self * Eigen::Quaterniond())
      .def(Eigen::Quaterniond() * py::self)
      .def("inverse", &Eigen::AngleAxisd::inverse)
      //.def("from_rotation_matrix", &Eigen::AngleAxisd::fromRotationMatrix)
      .def("from_rotation_matrix",
           [](Eigen::AngleAxisd& r, const Eigen::Matrix<double, 3, 3>& R) {
             r.fromRotationMatrix(R);
           })
      .def("to_rotation_matrix", &Eigen::AngleAxisd::toRotationMatrix)
      .def("rotation_matrix", &Eigen::AngleAxisd::toRotationMatrix)
      .def("matrix", &Eigen::AngleAxisd::toRotationMatrix)
      .def_property_readonly("R", &Eigen::AngleAxisd::toRotationMatrix)
      .def_static("ientity", &Eigen::AngleAxisd::Identity);

  py::enum_<Eigen::TransformTraits>(m, "TransformTraits")
      .value("Isometry", Eigen::Isometry)
      .value("Affine", Eigen::Affine)
      .value("AffineCompact", Eigen::AffineCompact)
      .value("Projective", Eigen::Projective)
      .export_values();

  templatedEigenIsometry<double, 2, Eigen::Isometry>(m, "Isometry2d");
  templatedEigenIsometry<double, 3, Eigen::Isometry>(m, "Isometry3d");

  /*templatedEigenTransform<double, 2, Eigen::Projective>(m, "Projective2d");
  templatedEigenTransform<double, 3, Eigen::Projective>(m, "Projective3d");

  templatedEigenTransform<double, 2, Eigen::Affine>(m, "Affine2d");
  templatedEigenTransform<double, 3, Eigen::Affine>(m, "Affine3d");*/
}

}  // end namespace g2o