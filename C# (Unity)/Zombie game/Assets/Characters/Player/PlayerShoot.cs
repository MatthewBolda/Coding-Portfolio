using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerShoot : MonoBehaviour
{

    public PlayerWeapon weapon;
    public WeaponManager WeaponManager;
    
    [SerializeField]
    private Camera cam;
    [SerializeField]
    private LayerMask mask;



    void Start ()
    {
        if (cam == null)
        {
            Debug.LogError("PlayerShoot: No camera referenced!");
            this.enabled = false;
        }
    }

    void Update ()
    {
        if (Input.GetButtonDown("Fire1"))
        {
            Shoot();
        }
    }

    void Shoot ()
    {
        RaycastHit _hit;
        if (Physics.Raycast(cam.transform.position, cam.transform.forward, out _hit, weapon.range, mask) )
        {
            //debugger, we hit something
            Debug.Log("hit " + _hit.collider.name);
            ZombieAI target = _hit.transform.GetComponent<ZombieAI>();
            if (target != null)
            {
                target.TakeDamage(PlayerWeapon.damage);
            }
        }
    }

}
