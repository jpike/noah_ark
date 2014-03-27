#pragma once

/// Holds physics-related functionality.
namespace PHYSICS
{
    ///////////////////////////////////////////////////////////
    /// \brief  Responsible for controlling game physics (mainly collisions).
    ///////////////////////////////////////////////////////////
    class PhysicsSystem
    {
    public:
        /// @brief  Constructor.
        explicit PhysicsSystem();
        /// @brief  Destructor.
        ~PhysicsSystem();

    private:
        PhysicsSystem(const PhysicsSystem& systemToCopy);  ///< Private to disallow copying.
        PhysicsSystem& operator= (const PhysicsSystem& rhsSystem); ///< Private to disallow assignment.
    };
}